const menuOpenButton = document.querySelector("#menu-open-button");
const menuCloseButton = document.querySelector("#menu-close-button");

// ── auto detect which server you're on ───────────────
const isLiveServer = window.location.port === '5500' || 
                     window.location.port === '5501';

menuOpenButton.addEventListener("click", () => {
    document.body.classList.toggle("show-mobile-menu");
});

menuCloseButton.addEventListener("click", () => menuOpenButton.click());

// ── API + state ───────────────────────────────────────
const API = 'http://localhost:8080/api';
let customers = [];
let sortAsc = true;

// ── Avatar helpers ────────────────────────────────────
function avClass(name) {
    let h = 0;
    for (let c of name) h = (h * 31 + c.charCodeAt(0)) % 8;
    return 'av' + h;
}

function initials(name) {
    return name.trim().split(/\s+/).map(w => w[0]).join('').toUpperCase().slice(0, 2);
}

function avHtml(name, size) {
    size = size || 34;
    return `<div class="avatar ${avClass(name)}" style="width:${size}px;height:${size}px">${initials(name)}</div>`;
}

function esc(s) {
    return String(s)
        .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;');
}

// ── Load all customers ────────────────────────────────
async function loadCustomers() {
    try {
        const r = await fetch(API + '/customers');
        customers = await r.json();
        renderStats();
        renderTable();
    } catch (e) {
        document.getElementById('table-wrap').innerHTML =
            '<div class="empty">Cannot connect to server at localhost:8080.<br>Make sure your C++ backend is running.</div>';
    }
}

// ── Stats ─────────────────────────────────────────────
function renderStats() {
    const n      = customers.length;
    const recent = n ? customers[customers.length - 1].name : '—';

    document.getElementById('s-total').textContent   = n;
    document.getElementById('s-recent').textContent  = recent;
    document.getElementById('s-results').textContent = '—';
}

// ── Render table ──────────────────────────────────────
function renderTable() {
    if (!customers.length) {
        document.getElementById('table-wrap').innerHTML = '<div class="empty">No customers found.</div>';
        return;
    }

    const rows = customers.map(c => `
        <tr>
          <td>
            <div class="td-cust">
              ${avHtml(c.name, 34)}
              <div>
                <div class="td-name">${esc(c.name)}</div>
                <div class="td-id">ID ${c.id}</div>
              </div>
            </div>
          </td>
          <td>${esc(c.phone)}</td>
          <td style="color:#4f86c6">${esc(c.email)}</td>
          <td style="color:#888">${esc(c.address)}</td>
          <td><button class="remove-btn" onclick="removeCustomer(${c.id},'${esc(c.name)}')">Remove</button></td>
        </tr>`).join('');

    document.getElementById('table-wrap').innerHTML = `
        <table>
          <thead><tr>
            <th>Customer</th><th>Phone</th><th>Email</th><th>Address</th><th></th>
          </tr></thead>
          <tbody>${rows}</tbody>
        </table>`;
}

// ── Sort ──────────────────────────────────────────────
function sortByName() {
    sortAsc = !sortAsc;
    customers.sort((a, b) => sortAsc
        ? a.name.localeCompare(b.name)
        : b.name.localeCompare(a.name));
    renderTable();
}

function sortById() {
    customers.sort((a, b) => a.id - b.id);
    renderTable();
}

// ── Add customer ──────────────────────────────────────
async function addCustomer() {
    const name    = document.getElementById('inp-name').value.trim();
    const phone   = document.getElementById('inp-phone').value.trim();
    const email   = document.getElementById('inp-email').value.trim();
    const address = document.getElementById('inp-address').value.trim();

    if (!name || !phone) {
        alert('Please fill in at least name and phone.');
        return;
    }

    try {
        const r = await fetch(API + '/customers', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name, phone, email, address })
        });
        const d = await r.json();
        if (d.success) {
            ['inp-name', 'inp-phone', 'inp-email', 'inp-address']
                .forEach(id => document.getElementById(id).value = '');
            loadCustomers();
        }
    } catch (e) { alert('Server error.'); }
}

// ── Search ────────────────────────────────────────────
async function doSearch() {
    const q   = document.getElementById('search-inp').value.trim();
    const box = document.getElementById('search-results');

    if (!q) {
        box.innerHTML = '';
        document.getElementById('s-results').textContent = '—';
        return;
    }

    try {
        const r       = await fetch(API + '/customers/search/' + encodeURIComponent(q));
        const results = await r.json();

        document.getElementById('s-results').textContent = results.length || 0;

        if (!results.length) {
            box.innerHTML = `<div style="font-size:12px;color:#555;padding:6px 0">No results for "${esc(q)}"</div>`;
            return;
        }

        box.innerHTML = results.map(c => `
            <div class="cust-row-result">
              ${avHtml(c.name, 34)}
              <div class="cust-inf">
                <div class="cname">${esc(c.name)}</div>
                <div class="cmeta">${esc(c.phone)} · ${esc(c.email)}</div>
              </div>
            </div>`).join('');
    } catch (e) {
        box.innerHTML = '<div style="color:#e74c3c;font-size:12px">Server error.</div>';
    }
}

// ── Remove customer ───────────────────────────────────
async function removeCustomer(id, name) {
    if (!confirm('Remove ' + name + '?')) return;
    try {
        await fetch(API + '/customers/' + id, { method: 'DELETE' });
        loadCustomers();
    } catch (e) { alert('Server error.'); }
}

loadCustomers();