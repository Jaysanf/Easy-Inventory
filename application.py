import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///inventory.db")

@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Take inventory"""
    if request.method == "GET":
        # gets company and user id and procedures
        id = session["user_id"]
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']
        procedures = db.execute("SELECT * FROM procedure WHERE company = :company;", company=company)
        return render_template("index.html",procedures=procedures)

    else:
        id = session["user_id"]
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']
        user = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['username']

        procedures = db.execute("SELECT * FROM procedure WHERE company = :company;", company=company)
        # Runs all the checks before
        for procedure in procedures:
            item = procedure["name"]
             # Checks everything for amount
            amount = (request.form.get(f"multiplier.{item}"))
            if not amount and procedure["multiplier"] != "":
                return apology(f"You must enter an amount for {item}")
            if procedure["multiplier"] != "":
                try:
                    amount = float(amount)
                except:
                    return apology(f"Amount must be a number and you must use a . for a decimal({item})")

            # checks everything for units
            unit = (request.form.get(f"unit.{item}"))
            if not unit:
                return apology(f"You must enter an amount of unit for {item}")
            try:
                unit = float(unit)
            except:
                return apology(f"Amount of unit must be a number and you must use a . for a decimal({item})")

            # checks everything for gamme
            gamma = request.form.get(f"gamma.{item}")
            if not gamma:
                return apology(f"You must enter a gamma for {item}")
            try:
                gamma = float(gamma)
            except:
                return apology(f"Gamma must be a number and you must use a . for a decimal({item})")
        # runs all the calculations
        for procedure in procedures:
            item = procedure["name"]
            if procedure["multiplier"] == "":
                amount = 0
            else:
                multiplier = procedure["multiplier"]
                amount = float(request.form.get(f"multiplier.{item}"))
                amount *=multiplier
            unit = float(request.form.get(f"unit.{item}"))
            gamma = float(request.form.get(f"gamma.{item}"))
            amount +=  unit
            diff = amount - gamma
            db.execute("INSERT INTO inventory (name, amount, gamma, diff, user, company) VALUES (:name, :amount, :gamma, :diff, :user, :company)", name=item, amount=amount, gamma=gamma, diff=diff, user=user, company=company)
        return redirect("/")



@app.route("/procedure", methods=["GET", "POST"])
@login_required
def procedure():
    if request.method == "GET":

        # check for creator or admin status
        if session["username"] != "creator" and session["username"] != "admin" :
            return apology("Must be creator or admin to do this action")

        # gets company and user id and procedures
        id = session["user_id"]
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']
        procedures = db.execute("SELECT * FROM procedure WHERE company = :company;", company=company)

        return render_template("procedure.html",procedures=procedures)
    # By post
    else:
        # check for creator or admin status
        if session["username"] != "creator" and session["username"] != "admin" :
            return apology("Must be creator or admin to do this action")
        id = session["user_id"]

        # Get info from the submit
        item = request.form.get("item")
        action= request.form.get("action")
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']

        # if no action has been chosen return apology
        if action == "0":
            return apology(" You must choose an Action")

        # Action that requires the form to be completed
        if action == "3" or action == "1":
            # Name and quantity is required
            name = request.form.get("name")
            if not name:
                return apology("You must provide a Name for that item")
            quantity = request.form.get("quantity")
            if not quantity:
                return apology("You must provide a quantity for that item")

            # Multiplier and multiplier item not required, still look for a valid float number
            multiplier = str(request.form.get("multiplier"))
            if not multiplier:
                pass
            else:

                try:
                    multiplier = float(multiplier)
                except:
                    return apology("Multiplier must be a number and you must use a . for a decimal")
            multiplier_item = request.form.get("multiplier_item")



        # Action (3) Add a new item to the list
        if action =="3":
            # looks for an item with the same name
            if len(db.execute("SELECT * FROM procedure WHERE name = :name AND company = :company", name=name, company=company)) != 0 :
                return apology("This item name is already taken")

            # Insert into db
            db.execute("INSERT INTO procedure (name, quantity, multiplier, multiplier_item, company) VALUES (:name, :quantity, :multiplier, :multiplier_item, :company)",name=name, quantity=quantity, multiplier=multiplier, multiplier_item=multiplier_item, company=company)
            return redirect("/procedure")

        else:
            if item == "0":
                return apology(" You must choose an item to perform this action")

            # Action(1) Edit an item
            if action =="1":
                # looks for an item with the same name
                if len(db.execute("SELECT * FROM procedure WHERE name = :name AND company = :company", name=name, company=company)) != 0 and name != item:
                    return apology("This item name is already taken")

                # Update DB
                db.execute("UPDATE procedure SET name = :name, quantity = :quantity, multiplier = :multiplier, multiplier_item = :multiplier_item WHERE name = :item AND company= :company;",name=name, quantity=quantity, multiplier=multiplier, multiplier_item=multiplier_item, item=item, company=company,)
                return redirect("/procedure")

            # Action(2) Delete an item
            if action =="2":
                # Update DB
                db.execute("DELETE FROM procedure WHERE name = :item AND company= :company;",item=item, company=company,)
                return redirect("/procedure")

    return apology("TODO")




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

@app.route("/change_company", methods=["GET", "POST"])
@login_required
def change_company():
    if request.method == "GET":
        if session["username"] != "creator":
            return apology("Must be creator to do this action")
        return render_template("change-company.html")
    else:
        if session["username"] != "creator":
            return apology("Must be creator to do this action")
        id = session["user_id"]
        # gets company
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']
         # Gets username and checks if it already exists or the username is blank
        company_password = db.execute("SELECT hash FROM companies WHERE name = :company;", company=company)
        if not check_password_hash(company_password[0]["hash"], request.form.get("oldpassword")):
            return apology(f"You must provide a password or Your password is incorrect.")

        # Gets password and confirmation and checks if they match or they blank
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation or password != confirmation:
            return apology("You must provide both password and confirmation and they must be the same")

        # If it passes the checks, insert new user into DB
        hashed = generate_password_hash(password)
        db.execute("UPDATE companies SET hash = :hashed WHERE name = :company;", company=company, hashed=hashed)
        return redirect("/")
    return apology("TODO")


@app.route("/manage", methods=["GET", "POST"])
@login_required
def manage():
    if request.method == "GET":
        if session["username"] != "creator":
            return apology("Must be creator to do this action")
        id = session["user_id"]
        company = db.execute("SELECT * FROM users WHERE id = :id;", id=id)[0]['company']
        user_list = db.execute("SELECT * FROM users WHERE company = :company;", company=company)
        return render_template("manage.html", user_list=user_list)
    else:
        # check for creator status
        if session["username"] != "creator":
            return apology("Must be creator to do this action")
        id = session["user_id"]
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']
        # checks if everything has been selected
        user = request.form.get("user")
        if user == "0":
            return apology(" You must choose a User")
        user_state = db.execute("SELECT * FROM users WHERE username = :name AND company = :company;", name=user, company=company)[0]

        action= request.form.get("action")
        if action == "0":
            return apology(" You must choose an Action")


        # Action(1) Admin or Unadmin a user
        # Check for admin attributes of user
        if action =="1":
            if user_state["admin"] == 0:
                db.execute("UPDATE users SET admin = 1 WHERE username = :name AND company = :company;", name=user, company=company)
                return redirect("/manage")
            else:
                db.execute("UPDATE users SET admin = 0 WHERE username = :name AND company = :company;", name=user, company=company)
                return redirect("/manage")

        # Action(2) Creator or Uncreator a user
        # Check for creator attributes of user
        if action =="2":
            if user_state["creator"] == 0:
                db.execute("UPDATE users SET creator = 1 WHERE username = :name AND company = :company;", name=user, company=company)
                return redirect("/manage")
            else:
                if len(db.execute("SELECT * FROM users WHERE creator = 1 AND company = :company;",  company=company)) == 1 and id == user_state["id"]:
                    return apology("There must be at least 2 creator in the company to uncreator yourself")
                elif id == user_state["id"]:
                    return render_template("confirm.html", message="You are about to uncreator yourself!")
                else:
                    db.execute("UPDATE users SET creator = 0 WHERE username = :name AND company = :company;", name=user, company=company)
                return redirect("/manage")

        # Action(3) Delete a user
        if action =="3":
            if len(db.execute("SELECT * FROM users WHERE creator = 1 AND company = :company;", company=company)) == 1 and id == user_state["id"]:
                    return apology("There must be at least 2 creator in the company to delete yourself")
            elif id == user_state["id"]:
                return render_template("confirm.html", message="You are about to delete yourself!")
            else:
                db.execute("DELETE FROM users WHERE username = :name AND company = :company;", name=user, company=company)
            return redirect("/manage")

        return apology("todo")
    return apology("TODO")


@app.route("/confirm", methods=["GET", "POST"])
@login_required
def confirm():
    if request.method == "GET":
        return render_template("confirm.html")
    else:
        action = request.form.get("button")
        if action == "0":
            db.execute("UPDATE users SET creator = 0 WHERE id = :id;", id=session["user_id"])
            if db.execute("SELECT creator FROM users WHERE id=:id", id=session["user_id"])[0]["creator"] == 1:
                session['username'] = 'creator'
            elif db.execute("SELECT admin FROM users WHERE id=:id", id=session["user_id"])[0]["admin"] == 1:
                session['username'] = 'admin'
            else:
                session['username'] = 'user'
            return redirect("/")
        else:
            db.execute("DELETE FROM users WHERE id = :id;", id= session["user_id"])
            return redirect("/logout")








@app.route("/history", methods=["GET", "POST"])
@login_required
def history():
    """Show history of inventory"""
    if request.method == "GET":
        # important info
        id = session["user_id"]
        company = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['company']
        user = db.execute("SELECT * FROM users WHERE id = :id", id=id)[0]['username']
        inventories = db.execute("SELECT * FROM inventory WHERE company = :company", company=company)
        inventories_sorted = []
        date = ""
        if not inventories:
            return apology("No inventory has been taken yet")
        for item in inventories:
            # Makes sure the seconds doesnt fuck up inventory
            item["date"] = item["date"][:-2] + "00"

            if date != item["date"]:
                try:
                    inventories_sorted.append(one_inventory)
                except:
                    pass
                one_inventory = []
                date = item["date"]
            one_inventory.append(item)
        inventories_sorted.append(one_inventory)

        return render_template("history.html", inventories=inventories_sorted[::-1])
    else:
        deleted = request.form.get("item")
        if deleted == "0":
            return apology("Must select an Inventory to delete")
        else:
            deleted = deleted[:-2] + "%"
            db.execute("DELETE FROM inventory WHERE date LIKE :date ", date=deleted)
            return redirect("/history")


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

        # Puts the correct tag to the session, enables new function the higher the rank
        if db.execute("SELECT creator FROM users WHERE id=:id", id=session["user_id"])[0]["creator"] == 1:
            session['username'] = 'creator'
        elif db.execute("SELECT admin FROM users WHERE id=:id", id=session["user_id"])[0]["admin"] == 1:
            session['username'] = 'admin'
        else:
            session['username'] = 'user'

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

        # Gets company name and password and check through database if both matches
        companyname = request.form.get("companyname")
        companypassword = request.form.get("companypassword")

        rows = db.execute("SELECT * FROM companies WHERE name = :username",
                          username=companyname)

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], companypassword):
            return apology("invalid company name or password", 403)


        # If it passes the checks, insert new user into DB
        hashed = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash, admin, creator, company) VALUES (:username, :hashed, 0, 0,:company)", username=username, hashed=hashed, company=companyname)
        return redirect("/")

@app.route("/register_new_company", methods=["GET", "POST"])
def register_new_company():
    """Register user"""
    if request.method == "GET":
        return render_template("register_new_company.html")
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

        # Get name of company and checks if it already exists or blank
        companyname = request.form.get("companyname")
        if not companyname or (len(db.execute('SELECT name FROM companies WHERE name = :companyname', companyname=companyname))) != 0:
            return apology("You must provide a Company's name or this name is already taken.")

        # gets password and confirmation and check if they match
        companypassword = request.form.get("companypassword")
        companyconfirmation = request.form.get("companypasswordconfirmation")
        if not companypassword or not companyconfirmation or companypassword != companyconfirmation:
            return apology("You must provide both password and confirmation and they must be the same")


        # If it passes the checks, insert new user and company into DB
        hashed_user = generate_password_hash(password)
        hashed_company = generate_password_hash(companypassword)
        db.execute("INSERT INTO users (username, hash, creator, admin, company) VALUES (:username, :hashed, 1, 1, :company)", username=username, hashed=hashed_user, company=companyname)
        db.execute("INSERT INTO companies (name, hash) VALUES (:name, :hashed)", name=companyname, hashed=hashed_company)
        return redirect("/")






def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
