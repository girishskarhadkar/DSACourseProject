from flask import Flask, jsonify,request
import requests
from flask_cors import CORS
import ctypes

app = Flask(__name__)
CORS(app)  # Allow all origins

# Load your DLL
lib = ctypes.CDLL("./courseproject.dll")
#return type for show theatre
lib.showtheater.restype = ctypes.POINTER(ctypes.c_int)

#return type for getrows
lib.get_rows.restype = ctypes.c_int

#return type for get columns 
lib.get_cols.restype = ctypes.c_int

#return type and arguement type for book seats
lib.bookseat.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_char_p,ctypes.c_char]
lib.bookseat.restype = ctypes.c_int

#return type and arguement type for cancel seat
lib.cancelseat.argtypes = [ctypes.c_int, ctypes.c_int]
lib.cancelseat.restype = ctypes.c_int

#return type for bookhistory
lib.bookhistory.restype = ctypes.c_char_p

#return type and arguement type for searchtickets
lib.searchtickets.argtypes=[ctypes.c_char_p]
lib.searchtickets.restype=ctypes.c_char_p


#return type for TotalSeatsBooked
lib.TotalSeatsBooked.restype=ctypes.c_int

#return type for TotalRevenue
lib.TotalRevenue.restype=ctypes.c_int

@app.route("/total_revenue",methods=["GET"])
def total_revenue():
    count=lib.TotalSeatsBooked()
    total=lib.TotalRevenue()
    str1=f"The total Revenue for {count} seats is Rs. {total} "
    return jsonify({"Message":str1})






@app.route("/search_tickets", methods=["GET"])
def search_tickets():
     name = request.args.get("name")
     if name is None:
        return jsonify({"status": "error", "message": "Missing parameters"}), 400
     history = lib.searchtickets(name.encode("utf-8"))
     history_str = history.decode("utf-8")
     formatted = history_str.replace("\n", "<br>")
     return jsonify({"history": formatted})




@app.route("/booking_history", methods=["GET"])
def booking_history():
    history = lib.bookhistory()
    history_str = history.decode("utf-8")
    formatted = history_str.replace("\n", "<br>")
    return jsonify({"history": formatted})


@app.route("/get_seats")
def get_seats():
    rows = lib.get_rows()
    cols = lib.get_cols()
    ptr = lib.showtheater()

    # Convert to Python nested list
    matrix = []
    for i in range(rows):
        row = [ptr[i * cols + j] for j in range(cols)]
        matrix.append(row)

    return jsonify(matrix)


@app.route("/book_seat", methods=["POST"])
def book_seat():
    name = request.args.get("name")
    row = request.args.get("row", type=int)
    col = request.args.get("col", type=int)
    snacks=request.args.get("snacks")

    if snacks is None:
        snacks="N"

    if name is None or row is None or col is None:
        return jsonify({"status": "error", "message": "Missing parameters"}), 400
    result = lib.bookseat(row, col, name.encode("utf-8"),snacks.encode("utf-8"))
    if result == 0:
        return jsonify({"status": "success", "message": f"Seat ({row},{col}) booked for {name}!"})
    elif result == 1:
        return jsonify({"status": "error", "message": "Invalid seat position!"})
    elif result == 2:
        return jsonify({"status": "success", "message": "Seat already booked! No Change in snacks requirements"})
    elif result == 3:
        return jsonify({"status": "success", "message": "Seat Booking is updated from snacks required to snacks not required "})
    elif result == 4:
        return jsonify({"status": "success", "message": "Seat Booking is updated from snacks not required to snacks required "})
    else:
        return jsonify({"status": "error", "message": "Unknown error."})
    
@app.route("/cancel_seat", methods=["DELETE"])
def cancel_seat():
    # Get row and column from query parameters
    row = request.args.get("row", type=int)
    col = request.args.get("col", type=int)

    if row is None or col is None:
        return jsonify({"status": "error", "message": "Missing row or column parameter"}), 400

    result = lib.cancelseat(row, col)

    # Map C function return codes to messages
    if result == 0:
        return jsonify({"status": "success", "message": f"Seat ({row},{col}) reservation canceled successfully!"})
    elif result == 1:
        return jsonify({"status": "error", "message": "Invalid seat position!"})
    elif result == 2:
        return jsonify({"status": "error", "message": "This seat is already vacant!"})
    elif result == 3:
        return jsonify({"status": "error", "message": "Booking not found in history!"})
    else:
        return jsonify({"status": "error", "message": "Unknown error."})

if __name__ == "__main__":
    app.run(debug=True)
