This Website is my submission (Jérémie Sanfaçon) to the final project of
the class CS50 I took in 2020. I am a cook and a substitute manager at a restaurant.
Each time I close the restaurant I have to go a head and do the inventory of important
things in the restaurant. It was a very boring task since you had to write them down on
a sheet of paper and do some math.(I.e: For each box of fries you would multiply by 6
because there are 6 bags of fries in a box) I decided, last year, when I got into python,
to write a small program to go around and take the inventory, but I still had to write
it down on paper and you could not really remove or add item easily. This is the reason I
decided to do my website Easy Inventory.


When you log in on the website you have a similar interface of the CS50 Finance since
I've used the backbones of the website. You also have the option to Register a New account.
When you try to register, the website will prompt you for a company username and a
company password. If you don't have an existing company, you will be able to register as
a new company and create the company name and company password.

Once you are logged in, you will arrive at blank page. When you have fill
out some information about how you wish to take the inventory, this will be a form
to take the inventory.

As the person who created the company, you will have the session_username Creator. This
will grant you access to some features like change company's password or manage company's
user. In manage company's user, you are able to delete a user at your company, give or
remove the  session_username Creator to a user and give or remove the
sessio_username Admin to a user. Don't worry, you are not able to "Uncreator"/Delete
yourself if there is not another Creator at your company.


If you have the session_username Admin or Creator, you will be able to access Manage
inventory Procedure. This page will give you access to a list of your existing procedure.
You can then go ahead and create a new procedure by filling in a form: The name of the item,
the units of the item, and if you wish to have a multiplier(I.e: like with the box of fries)
you can add one and the multiplier units. You will also be able to edit existing items and
also delete them.

Once you have your inventory procedures all set up, you will notice that the "/" has now
a form with all the items you put in your inventory procedure.Note that any user can take
the inventory. You will have to go ahead and insert the amount of box of Item X,
the number of units of Item X, and the Gamma. The gamma is the amount of Item X
you are supposed to have. More than often, this number is bigger than the real
amount of Item X because of loss. Once you've taken your inventory and
clicked the button confirm you can go ahead and click the History button.

In the history page, you will see many tables of each inventory that was taken. The
title of the table will contain the name of the user who took the inventory and date
when he took it. In each table you have the item name, the amount, the gamma, and the
difference between both. If you are an admin or a creator, you can select a table by
selecting the date in a drop down menu and then delete it. So if a user makes a mistake,
he just have to resubmit a form and an admin or creator can go ahead and delete the table
with a mistake.

** Flask run to start