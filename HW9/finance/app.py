import os
import time
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_stocks = db.execute("SELECT * FROM user_stocks WHERE id=?;", session["user_id"])
    total_val = 0
    for i in range(len(user_stocks)):
        stock = lookup(user_stocks[i]["stock"])
        user_stocks[i]["price"] = stock["price"]
        user_stocks[i]["tot_value"] = stock["price"] * user_stocks[i]["shares"]
        total_val += stock["price"] * user_stocks[i]["shares"]
    user = db.execute("SELECT * FROM users WHERE id = ?;", session["user_id"])
    cash = user[0]["cash"]
    total_val += cash
    return render_template("index.html", user_stocks=user_stocks, cash=cash, total_val=total_val)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # check stock symbol is exist or not
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("stock doesn't exist")

        # Check shares is positive integer
        try:
            shares = int(request.form.get("shares"))
            if shares <= 0:
                return apology("please enter a positive number of shares")
        except:
            return apology("please enter a integer number of shares")
        # load user bank account
        user = db.execute("SELECT * FROM users WHERE id = ?;", session["user_id"])
        cash = user[0]["cash"]
        # check if user's cash enough
        if cash < (quote["price"] * shares):
            return apology("Cash not enough")
        # load user's stock account
        user_stocks = db.execute("SELECT * FROM user_stocks WHERE id=? AND stock=?;",
                                 session["user_id"], request.form.get("symbol"))
        # update user's cash should be
        user_cash = user[0]["cash"] - (quote["price"] * shares)
        # update user's stock and cash account
        action_time = time.asctime(time.gmtime(time.time()))
        try:
            if user_stocks:
                user_shares = user_stocks[0]["shares"] + shares
                db.execute("UPDATE user_stocks SET shares=? WHERE id=? AND stock=?;", user_shares,
                           session["user_id"], request.form.get("symbol"))
            else:
                db.execute("INSERT INTO user_stocks (id, stock, shares) VALUES (?, ?, ?);",
                           session["user_id"], request.form.get("symbol"), shares)
            db.execute("UPDATE users SET cash=? WHERE id=?;", user_cash, session["user_id"])
            # Record into history
            db.execute("INSERT INTO history (id, stock, price, action, shares, time) VALUES (?, ?, ?, ?, ?, ?);",
                       session["user_id"], request.form.get("symbol"), quote["price"], "buy", shares, action_time)
            # return to homepage
            return redirect("/")
        except:
            return apology("Something error")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE id=? ORDER BY time DESC;", session["user_id"])

    return render_template("history.html", history=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("stock symbol doesn't exist")
        return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=symbol)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Check if the username existed
        elif (db.execute("SELECT * FROM users WHERE username = ?;", request.form.get("username"))):
            return apology("username existed, please use another username.")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Confirm the password
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password confirmation failed")

        # Insert user into database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?);", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Remenber user login
        rows = db.execute("SELECT * FROM users WHERE username = ?;", request.form.get("username"))
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # get the stock symbol, shares that user want to sell
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("please choose a stock")
        # Get stock current price
        stock = lookup(symbol)
        stock_price = stock["price"]
        # load user bank account
        user = db.execute("SELECT * FROM users WHERE id = ?;", session["user_id"])
        # load user's stock account
        user_stocks = db.execute("SELECT * FROM user_stocks WHERE id=? AND stock=?;", session["user_id"], symbol)
        # Check shares is positive integer
        try:
            if int(shares) <= 0:
                return apology("please enter a positive number of shares")
        except:
            return apology("please enter a integer number of shares")
        # check if user's stock shares enough
        if user_stocks[0]["shares"] < shares:
            return apology("Shares not enough")
        # update user's cash should be
        user_cash = user[0]["cash"] + (stock_price * shares)
        # update user's stock shares should be
        user_shares = user_stocks[0]["shares"] - shares
        # update user's stock and cash account
        try:
            if user_shares > 0:
                db.execute("UPDATE user_stocks SET shares=? WHERE id=? AND stock=?;",
                           user_shares, session["user_id"], request.form.get("symbol"))
            else:
                db.execute("DELETE FROM user_stocks WHERE id=? AND stock=?;", session["user_id"], symbol)
            db.execute("UPDATE users SET cash=? WHERE id=?;", user_cash, session["user_id"])
            # Record into history
            action_time = time.asctime(time.gmtime(time.time()))
            db.execute("INSERT INTO history (id, stock, price, action, shares, time) VALUES(?, ?, ?, ?, ?, ?);",
                       session["user_id"], symbol, stock_price, "sell", shares, action_time)
            # return to homepage
            return redirect("/")
        except:
            return apology("ERROR")

    user_stocks = db.execute("SELECT stock, shares FROM user_stocks WHERE id=?;", session["user_id"])
    return render_template("sell.html", user_stocks=user_stocks)