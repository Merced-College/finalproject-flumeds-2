const API = 'http://localhost:8080/api';
let quotes = [];

// ── Shared Helpers (Matching appointment.js) ──
function esc(s) {
    if (!s) return "";
    return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
}

// ── Quote Calculations (Matching quote.cpp) ──
function calculateLaborCost(hours, rate) {
    return hours * rate;
}

function calculateTotal(supply, laborHours, laborRate) {
    return supply + calculateLaborCost(laborHours, laborRate);
}

function estimatedProfit(charged, totalCost) {
    return charged - totalCost;
}

// ── Load Quotes ──
async function loadQuotes() {
    try {
        const r = await fetch(API + '/quotes');
        quotes = await r.json();
        renderTable();
    } catch (e) {
        // Dummy data for preview if backend is offline
        /*quotes = [{
            id: 1001, customerName: "John Smith", serviceType: "Oil Change", 
            vehicleDesc: "2020 Ford F150", supplyCost: 45.0, laborHours: 1.0, 
            laborRate: 85.0, amountCharged: 150.0, notes: "Regular maintenance"
        }];*/
        renderTable();
    }
}

// ── Render Table ──
function renderTable() {
    const body = document.getElementById('quote-list-body');
    if (quotes.length === 0) {
        body.innerHTML = '<tr><td colspan="8" style="text-align:center; padding:30px;">No quotes found</td></tr>';
        return;
    }

    body.innerHTML = quotes.map(q => {
        const laborCost = calculateLaborCost(q.laborHours, q.laborRate);
        const totalCost = calculateTotal(q.supplyCost, q.laborHours, q.laborRate);
        const profit = estimatedProfit(q.amountCharged, totalCost);

        return `
            <tr>
                <td>#${q.id}</td>
                <td>
                    <div style="font-weight:600;">${esc(q.customerName)}</div>
                    <div style="font-size:11px; color:#666;">${esc(q.customerPhone)}</div>
                </td>
                <td>
                    <div>${esc(q.serviceType)}</div>
                    <div style="font-size:11px; color:#888;">${esc(q.vehicleDesc)}</div>
                </td>
                <td>$${q.supplyCost.toFixed(2)}</td>
                <td>$${laborCost.toFixed(2)}</td>
                <td style="color:#D4AF37; font-weight:600;">$${q.amountCharged.toFixed(2)}</td>
                <td class="${profit >= 0 ? 'profit-positive' : 'profit-negative'}">
                    $${profit.toFixed(2)}
                </td>
                <td>
                    <button class="remove-btn" onclick="deleteQuote(${q.id})"><i class="fas fa-trash"></i></button>
                </td>
            </tr>
        `;
    }).join('');
}

// ── Create Quote (POST to Backend) ──
// ── Create Quote ──────────────────────────────────────
async function createQuote() {
    const quoteData = {
        customerName:  document.getElementById('q-customerName').value.trim(),
        customerPhone: document.getElementById('q-customerPhone').value.trim(),
        serviceType:   document.getElementById('q-serviceType').value.trim(),
        vehicleDesc:   document.getElementById('q-vehicleDesc').value.trim(),
        laborHours:    parseFloat(document.getElementById('q-laborHours').value) || 0,
        laborRate:     parseFloat(document.getElementById('q-laborRate').value) || 0,
        supplyCost:    parseFloat(document.getElementById('q-supplyCost').value) || 0,
        amountCharged: parseFloat(document.getElementById('q-amountCharged').value) || 0,
        notes:         document.getElementById('q-notes').value.trim(),
        date:          new Date().toISOString().split('T')[0]  // ← adds today's date
    };

    if (!quoteData.customerName || !quoteData.amountCharged) {
        alert("Please enter customer name and charged amount.");
        return;
    }

    try {
        const r = await fetch(API + '/quotes', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(quoteData)
        });
        const d = await r.json();
        if (d.success) {
            // clear all fields
            ['q-customerName','q-customerPhone','q-serviceType','q-vehicleDesc',
             'q-laborHours','q-laborRate','q-supplyCost','q-amountCharged','q-notes']
                .forEach(id => document.getElementById(id).value = '');
            loadQuotes();
        }
    } catch(e) { alert('Server error.'); }
}

// ── Delete Quote ──────────────────────────────────────
async function deleteQuote(id) {
    if (!confirm('Delete this quote?')) return;
    try {
        await fetch(API + '/quotes/' + id, { method: 'DELETE' });
        loadQuotes();
    } catch(e) { alert('Server error.'); }
}

// Mobile Menu logic (Matching appointment.js)
document.getElementById('menu-open-button').addEventListener('click', () => {
    document.body.classList.toggle('show-mobile-menu');
});

loadQuotes();