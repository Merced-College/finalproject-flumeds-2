const menuOpenButton = document.querySelector("#menu-open-button");
const menuCloseButton = document.querySelector("#menu-close-button");

// ── auto detect which server you're on ───────────────
const isLiveServer = window.location.port === '5500' || 
                     window.location.port === '5501';

menuOpenButton.addEventListener("click", () => {
    //toggle mobile menu 
    document.body.classList.toggle("show-mobile-menu");

});

//Close menu when the close button is clicked
menuCloseButton.addEventListener("click", () => menuOpenButton.click());

// ── invoices ──────────────────────────────────────────
const API = 'http://localhost:8080/api';
let invoices = [];
let currentFilter = 'all';
let sortField = 'date';
let sortAsc = false;

function esc(s) {
    return String(s)
        .replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;')
        .replace(/"/g,'&quot;');
}

async function loadInvoices() {
    try {
        const r = await fetch(API + '/invoices');
        invoices = await r.json();
        renderStats();
        renderTable();
        renderRecent();
    } catch(e) {
        document.getElementById('table-wrap').innerHTML =
            '<div class="empty">Cannot connect to server. Make sure your C++ backend is running.</div>';
    }
}

function renderStats() {
    const total      = invoices.length;
    const totalBilled = invoices.reduce((s, i) => s + i.amount, 0);
    const paidInvs   = invoices.filter(i => i.isPaid);
    const unpaidInvs = invoices.filter(i => !i.isPaid);
    const paidAmt    = paidInvs.reduce((s, i) => s + i.amount, 0);
    const unpaidAmt  = unpaidInvs.reduce((s, i) => s + i.amount, 0);

    document.getElementById('s-total').textContent       = total;
    document.getElementById('s-billed').textContent      = '$' + totalBilled.toLocaleString('en-US', {minimumFractionDigits:2});
    document.getElementById('s-paid').textContent        = '$' + paidAmt.toLocaleString('en-US', {minimumFractionDigits:2});
    document.getElementById('s-paid-count').textContent  = paidInvs.length + ' invoices';
    document.getElementById('s-unpaid').textContent      = '$' + unpaidAmt.toLocaleString('en-US', {minimumFractionDigits:2});
    document.getElementById('s-unpaid-count').textContent = unpaidInvs.length + ' invoices';
}

function renderRecent() {
    const recent = [...invoices]
        .sort((a, b) => b.id - a.id)
        .slice(0, 4);

    const div = document.getElementById('recent-list');
    if (!recent.length) {
        div.innerHTML = '<div class="empty" style="padding:10px 0">No invoices yet.</div>';
        return;
    }

    div.innerHTML = recent.map(inv => `
        <div class="recent-item">
            <div class="recent-info">
                <div class="recent-name">${esc(inv.customerName)}</div>
                <div class="recent-date">${inv.date}</div>
            </div>
            <div class="recent-right">
                <div class="recent-amount">$${inv.amount.toFixed(2)}</div>
                <span class="badge ${inv.isPaid ? 'badge-ft' : 'badge-red'}">${inv.isPaid ? 'Paid' : 'Unpaid'}</span>
            </div>
        </div>
    `).join('');
}

function renderTable() {
    let list = [...invoices];

    // filter
    if (currentFilter === 'paid')   list = list.filter(i => i.isPaid);
    if (currentFilter === 'unpaid') list = list.filter(i => !i.isPaid);

    // sort
    if (sortField === 'date') {
        list.sort((a, b) => sortAsc
            ? a.date.localeCompare(b.date)
            : b.date.localeCompare(a.date));
    } else if (sortField === 'amount') {
        list.sort((a, b) => sortAsc
            ? a.amount - b.amount
            : b.amount - a.amount);
    }

    if (!list.length) {
        document.getElementById('table-wrap').innerHTML =
            '<div class="empty">No invoices found.</div>';
        return;
    }

    document.getElementById('table-wrap').innerHTML = `
        <table>
            <thead>
                <tr>
                    <th>#</th>
                    <th>Customer</th>
                    <th>Date</th>
                    <th>Amount</th>
                    <th>Status</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
                ${list.map(inv => `
                    <tr>
                        <td style="color:#666">#${inv.id}</td>
                        <td>
                            <div style="font-weight:500;color:#d0d0d0">${esc(inv.customerName)}</div>
                            <div style="font-size:11px;color:#555">ID: ${inv.customerId}</div>
                        </td>
                        <td>${inv.date}</td>
                        <td style="font-weight:600;color:#d0d0d0">$${inv.amount.toFixed(2)}</td>
                        <td>
                            <span class="badge ${inv.isPaid ? 'badge-ft' : 'badge-red'}">
                                ${inv.isPaid ? 'Paid' : 'Unpaid'}
                            </span>
                        </td>
                        <td>
                            ${!inv.isPaid
                                ? `<button class="pay-btn" onclick="payInvoice(${inv.id})">Mark paid</button>`
                                : '<span style="color:#3ecf8e;font-size:13px">✓ Done</span>'
                            }
                        </td>
                    </tr>
                `).join('')}
            </tbody>
        </table>`;
}

function setFilter(f) {
    currentFilter = f;
    document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
    document.getElementById('f-' + f).classList.add('active');
    renderTable();
}

function sortByDate() {
    if (sortField === 'date') sortAsc = !sortAsc;
    else { sortField = 'date'; sortAsc = false; }
    renderTable();
}

function sortByAmount() {
    if (sortField === 'amount') sortAsc = !sortAsc;
    else { sortField = 'amount'; sortAsc = false; }
    renderTable();
}

async function createInvoice() {
    const customerId   = parseInt(document.getElementById('inp-customer-id').value) || 0;
    const customerName = document.getElementById('inp-customer-name').value.trim();
    const date         = document.getElementById('inp-date').value;
    const amount       = parseFloat(document.getElementById('inp-amount').value) || 0;

    if (!customerName || !date || !amount) {
        alert('Customer name, date and amount are required.');
        return;
    }

    try {
        const r = await fetch(API + '/invoices', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ customerId, customerName, date, amount })
        });
        const d = await r.json();
        if (d.success) {
            ['inp-customer-id','inp-customer-name','inp-date','inp-amount']
                .forEach(id => document.getElementById(id).value = '');
            loadInvoices();
        }
    } catch(e) { alert('Server error.'); }
}

async function payInvoice(id) {
    try {
        await fetch(API + '/invoices/pay/' + id, { method: 'PUT' });
        loadInvoices();
    } catch(e) { alert('Server error.'); }
}

loadInvoices();