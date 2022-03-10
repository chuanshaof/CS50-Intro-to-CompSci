import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

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
    # Get current cash from the users
    cash = (db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"]))[0]['cash']

    total = cash

    # Get shares and quantity from stocks
    shares = db.execute("SELECT symbol, SUM(shares) quantity FROM stocks WHERE user_id=:id GROUP BY symbol", id=session["user_id"])

    i = 0

    # Delete entry if quantity is or less
    for j in range(len(shares)):
        if shares[i]['quantity'] < 1:
            del shares[i]
        else:
            i += 1

    # Look up stock information and add/edit current dictionary
    for i in range(len(shares)):

        stock = lookup(shares[i]['symbol'])

        shares[i]['price'] = usd(stock['price'])
        shares[i]['name'] = stock['name']
        shares[i]['sum'] = usd(stock['price'] * shares[i]['quantity'])
        total = total + stock['price'] * shares[i]['quantity']

    # Render template
    return render_template("index.html", shares=shares, cash=usd(cash), total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure password was submitted
        elif not request.form.get("shares"):
            return apology("missing shares", 400)

        # Get shares and convert into an integer
        shares = int(request.form.get("shares"))

        # Ensure that shares is positive
        if (shares <= 0):
            return apology("must enter positive shares", 400)

        # Look up the API based on input
        stock = lookup(request.form.get("symbol"))

        # If searched symbol is not found
        if not stock:
            return apology("invalid symbol", 400)

        # Get current cash from the table
        cash = (db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"]))[0]['cash']

        # Get the cost of the shares
        cost = stock["price"] * shares

        # Get remainder
        remainder = cash - cost

        # Ensure sufficient cash
        if remainder < 0:
            return apology("insufficient cash", 400)
        else:
            db.execute("INSERT INTO stocks (user_id, symbol, price, shares, time) VALUES (?, ?, ? ,? ,?)",
                        session["user_id"],
                        stock["symbol"],
                        stock["price"],
                        shares,
                        datetime.now())

            # Update new cash remainder
            db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=remainder, id=session["user_id"])

        # Redirect user to home page
        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get shares and quantity from stocks
    shares = db.execute("SELECT * FROM stocks WHERE user_id=:id ORDER BY time DESC", id=session["user_id"])

    for share in shares:
        share['price'] = usd(share['price'])

    # Render template
    return render_template("history.html", shares=shares)

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
        flash("Logged in!")
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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Look up the API based on input
        stock = lookup(request.form.get("symbol"))

        # If searched symbol is not found
        if not stock:
            return apology("invalid symbol", 400)
        # Render to quoted.html
        else:
            return render_template("quoted.html",
                                    name = stock["name"],
                                    price = usd(stock["price"]),
                                    symbol = stock["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("missing username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("missing password", 400)

        # Ensure password confirmation is given
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Ensure password and confirmation matches
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password do not match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure that username has not been taken
        if len(rows) == 1:
            return apology("username has been taken", 400)

        # Insert into the database usename and hashed password
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                    request.form.get("username"),
                    generate_password_hash(request.form.get("password")))

        # Get the ID that was created
        rows = db.execute("SELECT id FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure password was submitted
        elif not request.form.get("shares"):
            return apology("missing shares", 400)

        # Get shares and convert into an integer
        shares = int(request.form.get("shares"))

        # Ensure that shares is positive
        if (shares <= 0):
            return apology("must enter positive shares", 400)

        quantity = db.execute("SELECT SUM(shares) quantity FROM stocks WHERE user_id=:id AND symbol=:symbol",
                                id=session["user_id"],
                                symbol=request.form.get("symbol"))

        if (shares > quantity[0]['quantity']):
            return apology("insufficient shares", 400)
        else:
            # Look up the API based on input
            stock = lookup(request.form.get("symbol"))

            db.execute("INSERT INTO stocks (user_id, symbol, price, shares, time) VALUES (?, ?, ? ,? ,?)",
                        session["user_id"],
                        stock["symbol"],
                        stock["price"],
                        -shares,
                        datetime.now())

            # Get current holding cash from users
            cash = (db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"]))[0]['cash']

            cash = cash + stock["price"] * shares

            # Update new cash remainder
            db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash, id=session["user_id"])

            # Redirect user to home page
            flash("Sold!")
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get shares and quantity from stocks
        shares = db.execute("SELECT symbol, SUM(shares) quantity FROM stocks WHERE user_id=:id GROUP BY symbol", id=session["user_id"])

        i = 0

        # Delete entry if quantity is or less
        for j in range(len(shares)):
            if shares[i]['quantity'] < 1:
                del shares[i]
            else:
                i += 1

        return render_template("sell.html", symbols=shares)


@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    """Change user profile"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if request.form['profile'] == 'changepassword':
            if not request.form.get("oldpassword"):
                return apology("missing old password", 400)

            elif not request.form.get("password"):
                return apology("missing new password", 400)

            # Ensure password confirmation is given
            elif not request.form.get("confirmation"):
                return apology("must confirm password", 400)

            # Ensure password and confirmation matches
            elif request.form.get("password") != request.form.get("confirmation"):
                return apology("password do not match", 400)

            # Get password hash from users
            hash = db.execute("SELECT hash FROM users WHERE id=:id",
                                id=session["user_id"])

            # Ensure old password is correct
            if not check_password_hash(hash[0]["hash"], request.form.get("oldpassword")):
                return apology("password is incorrect", 403)

            else:
                # Update new password
                db.execute("UPDATE users SET hash=:hash WHERE id=:id",
                            hash=generate_password_hash(request.form.get("password")),
                            id=session["user_id"])

                flash("Password changed!")

        elif request.form['profile'] == 'topup':
            if not request.form.get("topup"):
                return apology("missing top up value", 400)

            cash = int((db.execute("SELECT cash FROM users WHERE id=:id",
                                id=session["user_id"]))[0]['cash'])

            cash += int(request.form.get("topup"))

            db.execute("UPDATE users SET cash=:cash WHERE id=:id",
                        cash=cash,
                        id=session["user_id"])

            db.execute("INSERT INTO stocks (user_id, symbol, price, shares, time) VALUES (?, ?, ? ,? ,?)",
                        session["user_id"],
                        'TOP UP',
                        request.form.get("topup"),
                        '',
                        datetime.now())

            flash("Topped up!")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("profile.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
