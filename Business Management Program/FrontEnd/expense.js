const menuOpenButton = document.querySelector("#menu-open-button");
const menuCloseButton = document.querySelector("#menu-close-button");

menuOpenButton.addEventListener("click", () => {
    document.body.classList.toggle("show-mobile-menu");
});

menuCloseButton.addEventListener("click", () => menuOpenButton.click());



// ── expenses ──────────────────────────────────────────
const API = 'http://localhost:8080/api';
let expenses = [];
let filteredExpenses = [];

function esc(s) {
    if (!s) return '';
    return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;');
}

function badgeClass(cat) {
    const map = { Supplies:'b-supplies', Labor:'b-labor', Gas:'b-gas', Equipment:'b-equipment', Other:'b-other' };
    return map[cat] || 'b-other';
}

// ── load all expenses ─────────────────────────────────
async function loadExpenses() {
    try {
        const r = await fetch(API + '/expenses');
        expenses = await r.json();
        filteredExpenses = [...expenses];
        renderStats();
        renderCategoryTotals();
        renderTable(filteredExpenses);
    } catch(e) {
        document.getElementById('table-wrap').innerHTML =
            '<div class="empty">Cannot connect to server. Make sure your C++ backend is running.</div>';
    }
}

// ── render stat cards ─────────────────────────────────
function renderStats() {
    const total = expenses.reduce((s, e) => s + e.amount, 0);
    const count = expenses.length;
    const avg   = count ? total / count : 0;

    const now   = new Date();
    const ym    = now.getFullYear() + '-' + String(now.getMonth() + 1).padStart(2, '0');
    const month = expenses.filter(e => e.date && e.date.startsWith(ym))
                          .reduce((s, e) => s + e.amount, 0);

    document.getElementById('s-total').textContent = '$' + total.toFixed(2);
    document.getElementById('s-month').textContent = '$' + month.toFixed(2);
    document.getElementById('s-count').textContent = count;
    document.getElementById('s-avg').textContent   = '$' + avg.toFixed(2);
}

// ── render category totals ────────────────────────────
function renderCategoryTotals() {
    const cats = ['Supplies', 'Labor', 'Gas', 'Equipment', 'Other'];
    cats.forEach(cat => {
        const total = expenses
            .filter(e => e.category === cat)
            .reduce((s, e) => s + e.amount, 0);
        const el = document.getElementById('total_' + cat);
        if (el) el.textContent = '$' + total.toFixed(2);
    });
}

// ── render table ──────────────────────────────────────
function renderTable(list) {
    const wrap = document.getElementById('table-wrap');

    if (!list.length) {
        wrap.innerHTML = '<div class="empty">No expenses found.</div>';
        document.getElementById('grand-total').textContent = '$0.00';
        return;
    }

    const grandTotal = list.reduce((s, e) => s + e.amount, 0);
    document.getElementById('grand-total').textContent = '$' + grandTotal.toFixed(2);

    wrap.innerHTML = `
        <table>
            <thead>
                <tr>
                    <th>#</th>
                    <th>Date</th>
                    <th>Category</th>
                    <th>Description</th>
                    <th>Amount</th>
                    <th></th>
                </tr>
            </thead>
            <tbody>
                ${list.map(e => `
                    <tr>
                        <td style="color:#666">#${e.id}</td>
                        <td>${e.date}</td>
                        <td><span class="badge ${badgeClass(e.category)}">${esc(e.category)}</span></td>
                        <td>${esc(e.description)}</td>
                        <td class="amount-col">$${e.amount.toFixed(2)}</td>
                        <td><button class="remove-btn" onclick="removeExpense(${e.id})">Remove</button></td>
                    </tr>
                `).join('')}
            </tbody>
        </table>`;
}

// ── add expense ───────────────────────────────────────
async function addExpense() {
    const amount      = parseFloat(document.getElementById('inp-amount').value) || 0;
    const category    = document.getElementById('inp-category').value;
    const date        = document.getElementById('inp-date').value;
    const description = document.getElementById('inp-desc').value.trim();

    if (!amount || !category || !date) {
        alert('Amount, category and date are required.');
        return;
    }

    try {
        await fetch(API + '/expenses', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ amount, category, date, description })
        });

        document.getElementById('inp-amount').value   = '';
        document.getElementById('inp-category').value = '';
        document.getElementById('inp-date').value     = '';
        document.getElementById('inp-desc').value     = '';

        const msg = document.getElementById('exp-success');
        msg.classList.remove('hidden');
        setTimeout(() => msg.classList.add('hidden'), 2500);

        loadExpenses();
    } catch(e) { alert('Server error.'); }
}

// ── remove expense ────────────────────────────────────
async function removeExpense(id) {
    if (!confirm('Remove this expense?')) return;
    try {
        await fetch(API + '/expenses/' + id, { method: 'DELETE' });
        loadExpenses();
    } catch(e) { alert('Server error.'); }
}

// ── filter by category ────────────────────────────────
function filterByCategory() {
    const cat = document.getElementById('filter-category').value;
    filteredExpenses = cat ? expenses.filter(e => e.category === cat) : [...expenses];
    renderTable(filteredExpenses);
}

// ── filter by month ───────────────────────────────────
function filterByMonth() {
    const month = document.getElementById('inp-month').value;
    if (!month) return;
    filteredExpenses = expenses.filter(e => e.date && e.date.startsWith(month));
    renderTable(filteredExpenses);
}

// ── clear filter ──────────────────────────────────────
function clearFilter() {
    document.getElementById('inp-month').value = '';
    document.getElementById('filter-category').value = '';
    filteredExpenses = [...expenses];
    renderTable(filteredExpenses);
}

loadExpenses();