import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    id = session["user_id"]
    all_shares = db.execute("SELECT * FROM stocks WHERE id=:id; ", id=id)
    wallet = (db.execute("SELECT cash FROM users WHERE id = :id;", id=id)[0]["cash"])
    total_end = wallet
    for share in all_shares:
        # Lookup info on stock for each stock
        symbol = share["symbol"]
        stock_quote = lookup(symbol)

        # Get the number of share owned
        current = db.execute("SELECT shares FROM stocks WHERE id = :id AND symbol = :symbol;",id=session["user_id"], symbol=stock_quote["symbol"])[0]["shares"]

        # Gets the total price owned of a stock
        total = current * stock_quote["price"]

        # Add it to grand total
        total_end += total

        # Update my DB  with new info
        db.execute("UPDATE stocks SET price=:price, total=:total WHERE id =:id AND symbol = :symbol; ", price=usd(stock_quote["price"]), total=usd(total), id=session["user_id"], symbol=stock_quote["symbol"])

    return render_template("index.html", all_shares=all_shares, wallet=usd(wallet), total_end=usd(total_end))



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # Gets the info from symbol
        symbol = request.form.get("symbol")
        stock_quote = lookup(symbol)

        # Checks if stock exists
        if not stock_quote:
            return apology("Invalid symbol")

        # gets number of share
        shares = request.form.get("shares")

        # Try to see if shares is a positive integer
        try:
            shares = int(shares)
            if shares <= 0:
                raise Exception("Sorry, no numbers below zero")
        except:
            return apology("Number of shares must be a positive number without decimal")

        # Try to buy X number of shares and check if enough money in wallet
        wallet = db.execute("SELECT cash FROM users WHERE id = :id;", id=session["user_id"])[0]["cash"]
        price_to_pay = stock_quote["price"] * shares

        if (wallet - price_to_pay) < 0:
            return apology("Can't Afford")
        # Updates wallet
        wallet = (wallet - price_to_pay)

        # Updates cash from user
        db.execute("UPDATE users SET cash = :wallet WHERE id = :id;", wallet=wallet, id=session["user_id"])

        # Updates purchases history
        db.execute("INSERT INTO purchases (id, symbol, shares, price) VALUES (:id, :symbol, :shares, :price);",
        id=session["user_id"], symbol=stock_quote["symbol"], shares=shares, price=stock_quote["price"])

        # Updates Stocks
        # Checks if users has a stock
        if len(db.execute("SELECT * FROM stocks WHERE id = :id AND symbol = :symbol;",id=session["user_id"], symbol=stock_quote["symbol"])) == 1:
            current = db.execute("SELECT shares FROM stocks WHERE id = :id AND symbol = :symbol;",id=session["user_id"], symbol=stock_quote["symbol"])[0]["shares"]
            db.execute("UPDATE stocks SET shares = :new WHERE id =:id AND symbol = :symbol;", new=(current + shares) , id=session["user_id"], symbol=stock_quote["symbol"])
        else:
            db.execute("INSERT INTO stocks (id, symbol, shares, name) VALUES (:id, :symbol, :shares, :name);", id=session["user_id"], symbol=stock_quote["symbol"], shares=shares, name=stock_quote["name"] )


        # purchase has been completed
        return redirect("/")

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "GET":
        return render_template("change.html")
    else:
        id = session["user_id"]
         # Gets username and checks if it already exists or the username is blank
        user = db.execute("SELECT * FROM users WHERE id = :id", id=id)
        if not check_password_hash(user[0]["hash"], request.form.get("oldpassword")):
            return apology(f"You must provide a password or Your password is incorrect.")

        # Gets password and confirmation and checks if they match or they blank
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation or password != confirmation:
            return apology("You must provide both password and confirmation and they must be the same")

        # If it passes the checks, insert new user into DB
        hashed = generate_password_hash(password)
        db.execute("UPDATE users SET hash = :hashed WHERE id = :id", id=id, hashed=hashed)
        return redirect("/")
    return apology("TODO")










@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session["user_id"]
    all_trans = db.execute("SELECT * FROM purchases WHERE id=:id; ", id=id)
    all_trans = all_trans[::-1]
    return render_template("history.html", all_trans=all_trans)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        # Gets stock symbol and validate:
        symbol = request.form.get("symbol")
        stock_quote = lookup(symbol)
        if not stock_quote:
            return apology("Invalid symbol")
        return render_template("quoted.html", stock_quote=stock_quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        # Gets username and checks if it already exists or the username is blank
        username = request.form.get("username")
        if not username or (len(db.execute('SELECT username FROM users WHERE username = :username', username=username))) != 0:
            return apology("You must provide a username or Your username is already taken.")

        # Gets password and confirmation and checks if they match or they blank
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation or password != confirmation:
            return apology("You must provide both password and confirmation and they must be the same")

        # If it passes the checks, insert new user into DB
        hashed = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashed)", username=username, hashed=hashed)
        return redirect("/")






@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        id = session["user_id"]
        all_shares = db.execute("SELECT * FROM stocks WHERE id=:id; ", id=id)
        return render_template("sell.html", all_shares=all_shares)
    else:
        # gets the sthe stock to sell and the amount.
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Checks if stock is in portfolio
        id = session["user_id"]
        all_shares = db.execute("SELECT * FROM stocks WHERE id=:id; ", id=id)

         # Try to see if shares is a positive integer
        try:
            shares = int(shares)
            if shares <= 0:
                raise Exception("Sorry, no numbers below zero")
        except:
            return apology("Number of shares must be a positive number without decimal")

        for stock in all_shares:
            # finds the valid stock
            if stock["symbol"] == symbol:
                # checks if enough share to sell
                number_left = (stock["shares"] - shares)
                if number_left < 0:
                    return apology("Not enough shares in your portfolio")
                # passes all test so do the transaction
                wallet = db.execute("SELECT cash FROM users WHERE id = :id;", id=session["user_id"])[0]["cash"]
                price = lookup(symbol)["price"]
                money = price * shares
                # Updates wallet
                wallet = (wallet + money)

                # Updates cash from user
                db.execute("UPDATE users SET cash = :wallet WHERE id = :id;", wallet=wallet, id=session["user_id"])

                # Updates purchases history
                db.execute("INSERT INTO purchases (id, symbol, shares, price) VALUES (:id, :symbol, :shares, :price);",
                id=session["user_id"], symbol=stock["symbol"], shares= -shares, price=price)

                # Updates Stocks
                # Checks if users has a stock left or not
                if number_left != 0:
                    db.execute("UPDATE stocks SET shares = :new WHERE id =:id AND symbol = :symbol;", new=number_left , id=session["user_id"], symbol=stock["symbol"])
                else:
                    db.execute("DELETE FROM stocks WHERE id = :id AND symbol = :symbol;", id=session["user_id"], symbol=stock["symbol"])
                return redirect("/")

        else:
            return apology(f"Invalid symbol or no stock available")



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
