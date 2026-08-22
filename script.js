async function loadSeats() {
  try {
    const res = await fetch("http://127.0.0.1:5000/get_seats");
    const matrix = await res.json();
    const container = document.getElementById("seat-container");
    container.innerHTML = "";

    const table = document.createElement("table");
    const tr1 = document.createElement("tr");
    for (let i = 0; i < 10; i++) {
      if (i == 0) {
        const td1 = document.createElement("td");
        td1.textContent = "R/C";
        td1.className = "index";
        tr1.appendChild(td1);
      }
      const td = document.createElement("td");
      td.textContent = i + 1;
      td.className = "index";
      tr1.appendChild(td);
    }
    table.appendChild(tr1);

    matrix.forEach((row, i) => {
      const tr = document.createElement("tr");
      const td = document.createElement("td");
      td.textContent = i + 1;
      td.className = "index";
      tr.appendChild(td);

      row.forEach((seat) => {
        const td = document.createElement("td");
        td.textContent = seat;
        td.className = seat === 1 ? "booked" : "available";
        tr.appendChild(td);
      });
      table.appendChild(tr);
    });

    container.appendChild(table);

    const revenueRes = await fetch("http://127.0.0.1:5000/total_revenue");
    const revenueData = await revenueRes.json();

    let revenueDiv = document.getElementById("total-revenue");
    if (!revenueDiv) {
      revenueDiv = document.createElement("div");
      revenueDiv.id = "total-revenue";
      revenueDiv.className = "mt-3 p-2 border rounded bg-light";
      container.parentNode.insertBefore(revenueDiv, container.nextSibling);
    }

    revenueDiv.innerHTML = `<strong>Total Revenue:</strong> ${revenueData.Message}`;
  } catch (err) {
    console.error("Error loading seats:", err);
  }
}

document.addEventListener("DOMContentLoaded", () => {
  loadSeats();

  // ---- Toggle Between Book/Cancel ----
  const bookForm = document.getElementById("booking-form");
  const cancelForm = document.getElementById("cancel-form");
  const bookBtn = document.getElementById("show-book-form");
  const cancelBtn = document.getElementById("show-cancel-form");

  bookBtn.addEventListener("click", () => {
    bookForm.classList.remove("d-none");
    cancelForm.classList.add("d-none");
    bookBtn.classList.add("active");
    cancelBtn.classList.remove("active");
  });

  cancelBtn.addEventListener("click", () => {
    cancelForm.classList.remove("d-none");
    bookForm.classList.add("d-none");
    cancelBtn.classList.add("active");
    bookBtn.classList.remove("active");
  });

  // ---- Booking Logic ----
  document.getElementById("book-ticket").addEventListener("click", async () => {
    const name = document.getElementById("name").value.trim();
    const row = document.getElementById("row").value.trim();
    const col = document.getElementById("col").value.trim();
    const snacks = document.getElementById("snacks").value;

    if (!name || !row || !col) return alert("Please fill all fields!");

    try {
      const response = await fetch(
        `http://127.0.0.1:5000/book_seat?name=${encodeURIComponent(name)}&row=${row}&col=${col}&snacks=${snacks}`,
        { method: "POST" }
      );
      const result = await response.json();
      alert(result.message);
      loadSeats();
    } catch (error) {
      console.error("Error booking seat:", error);
      alert("Something went wrong!");
    }
  });

  // ---- Cancel Logic ----
  document.getElementById("cancel-ticket").addEventListener("click", async () => {
    const row = document.getElementById("cancel-row").value.trim();
    const col = document.getElementById("cancel-col").value.trim();

    if (!row || !col) return alert("Please enter row and column!");

    try {
      const res = await fetch(`http://127.0.0.1:5000/cancel_seat?row=${row}&col=${col}`, {
        method: "DELETE",
      });
      const data = await res.json();
      alert(data.message);
      loadSeats();
    } catch (err) {
      console.error("Error canceling seat:", err);
      alert("Something went wrong!");
    }
  });

  // ---- History ----
  document.getElementById("show-history").addEventListener("click", async () => {
    try {
      const res = await fetch("http://127.0.0.1:5000/booking_history");
      const data = await res.json();
      document.getElementById("history-textarea").value = data.history.replace(/<br>/g, "\n");
    } catch {
      document.getElementById("history-textarea").value = "Error loading history.";
    }
  });

  document.getElementById("clear-history").addEventListener("click", () => {
    document.getElementById("history-textarea").value = "";
  });

  // ---- Search ----
  document.getElementById("search-ticket").addEventListener("click", async () => {
    const name = document.getElementById("search-name").value.trim();
    const out = document.getElementById("search-results");
    if (!name) return alert("Enter name to search!");

    try {
      const res = await fetch(`http://127.0.0.1:5000/search_tickets?name=${encodeURIComponent(name)}`);
      const data = await res.json();
      out.value = data.history ? data.history.replace(/<br\s*\/?>/gi, "\n") : "No results.";
    } catch {
      out.value = "Error retrieving search results.";
    }
  });

  document.getElementById("clear-search").addEventListener("click", () => {
    document.getElementById("search-results").value = "";
  });

  // ---- Toggle All ----
  const toggleBtn = document.getElementById("toggle-all");
  const collapseIds = ["#booking-card", "#history-card", "#search-card"];
  const collapseElements = collapseIds.map(
    (id) => new bootstrap.Collapse(document.querySelector(id), { toggle: false })
  );
  let isOpen = false;

  toggleBtn.addEventListener("click", function () {
    if (isOpen) {
      collapseElements.forEach((el) => el.hide());
      toggleBtn.textContent = "Open Box Office";
      toggleBtn.classList.remove("btn-danger");
      toggleBtn.classList.add("btn-success");
    } else {
      collapseElements.forEach((el) => el.show());
      toggleBtn.textContent = "Close Box Office";
      toggleBtn.classList.remove("btn-success");
      toggleBtn.classList.add("btn-danger");
    }
    isOpen = !isOpen;
  });
});

loadSeats();
