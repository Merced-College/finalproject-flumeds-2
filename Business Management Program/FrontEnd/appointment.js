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
let appointments = [];

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
    return `<div class="avatar ${avClass(name)}" style="width:${size}px;height:${size}px;display:flex;align-items:center;justify-content:center;border-radius:50%;font-size:12px;font-weight:600;flex-shrink:0;">${initials(name)}</div>`;
}

function esc(s) {
    if (!s) return "";
    return String(s)
        .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;');
}

// ── Load all appointments ──────────────────────────────
async function loadAppointments() {
    try {
        const r = await fetch(API + '/appointments');
        appointments = await r.json();
        renderTable(appointments);
    } catch (e) {
        document.getElementById('table-wrap').innerHTML =
            '<div class="empty" style="text-align:center; padding:20px; color:#999;">Cannot connect to server at localhost:8080.<br>Make sure your C++ backend is running.</div>';
    }
}

// ── Render table ──────────────────────────────────────
function renderTable(data) {
    const tableBody = document.getElementById('appointment-list-body');
    
    if (!data || data.length === 0) {
        tableBody.innerHTML = `<tr><td colspan="8" class="empty" style="text-align:center; padding:20px; color:#999;">No appointments found</td></tr>`;
        return;
    }

    tableBody.innerHTML = data.map(a => {
        // Status logic based on appointment.h isCompleted field
        const statusText = a.isCompleted ? "Completed" : "Scheduled";
        const statusClass = a.isCompleted ? "pill-done" : "pill-scheduled";

        return `
        <tr>
          <td style="padding:10px;">${a.id}</td>
          <td style="padding:10px;">
            <div style="display:flex; align-items:center; gap:10px;">
              ${avHtml(a.customerName, 34)}
              <div>
                <div style="font-weight:500; color:#d0d0d0;">${esc(a.customerName)}</div>
                <div style="font-size:11px; color:#555;">ID ${a.customerId}</div>
              </div>
            </div>
          </td>
          <td style="padding:10px;">${esc(a.date)}</td>
          <td style="padding:10px; color:#D4AF37;">${esc(a.time)}</td>
          <td style="padding:10px;">${esc(a.serviceType)}</td>
          <td style="padding:10px; color:#888; font-size:12px; max-width:150px; overflow:hidden; text-overflow:ellipsis;">${esc(a.notes || '—')}</td>
          <td style="padding:10px;">
            <span class="status-pill ${statusClass}">${statusText}</span>
          </td>
          <td style="padding:10px;">
            <button class="remove-btn" onclick="removeAppointment(${a.id}, '${esc(a.customerName)}')">
                <i class="fas fa-trash"></i> Cancel
            </button>
          </td>
        </tr>`;
    }).join('');
}

// ── Add Appointment (POST) ───────────────────────────
async function bookAppointment() {
    const customerId   = document.getElementById('appointment-customer-id').value;
    const customerName = document.getElementById('appointment-customer-name').value.trim();
    const date         = document.getElementById('appointment-date').value;
    const time         = document.getElementById('appointment-time').value;
    const serviceType  = document.getElementById('appointment-service').value.trim();
    const notes        = document.getElementById('appointment-notes').value.trim();

    if (!customerName || !date || !time) {
        alert('Please fill in Name, Date, and Time.');
        return;
    }

    try {
        const r = await fetch(API + '/appointments', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ 
                customerId: parseInt(customerId) || 0, 
                customerName, 
                date, 
                time, 
                serviceType, 
                notes 
            })
        });
        const d = await r.json();
        if (d.success) {
            // Clear inputs
            ['appointment-customer-id', 'appointment-customer-name', 'appointment-service', 'appointment-notes']
                .forEach(id => document.getElementById(id).value = '');
            loadAppointments();
        } else {
            alert('Conflict detected or server rejected appointment.');
        }
    } catch (e) { alert('Server error.'); }
}

// ── Search/Filter by Date ─────────────────────────────
async function viewSchedule() {
    const qDate = document.getElementById('view-date-inp').value;
    if (!qDate) return;

    try {
        // Calling static getByDate functionality via backend endpoint
        const r = await fetch(`${API}/appointments/date/${qDate}`);
        const filtered = await r.json();
        renderTable(filtered);
    } catch (e) {
        alert('Error filtering by date.');
    }
}

function showAll() {
    loadAppointments();
}

// ── Remove/Cancel Appointment ─────────────────────────
async function removeAppointment(id, name) {
    if (!confirm(`Cancel appointment for ${name}?`)) return;
    try {
        const r = await fetch(`${API}/appointments/${id}`, { method: 'DELETE' });
        const d = await r.json();
        if (d.success) loadAppointments();
    } catch (e) { alert('Server error.'); }
}

// load next appointment in queue
async function loadNextAppointment() {
    const r    = await fetch(API + '/appointments/next');
    const data = await r.json();
    const div  = document.getElementById('next-appointment');

    if (data.message) {
        div.innerHTML = '<div class="empty">No upcoming appointments</div>';
        return;
    }

    div.innerHTML = `
        <div class="next-appt-card">
            <div class="next-label">Next up</div>
            <div class="next-name">${data.customerName}</div>
            <div class="next-meta">${data.date} at ${data.time}</div>
            <div class="next-service">${data.serviceType}</div>
            <button class="btn-main" onclick="completeNext()">
                Mark Complete
            </button>
        </div>`;
}

// complete next appointment in queue
async function completeNext() {
    await fetch(API + '/appointments/next/complete', { method: 'PUT' });
    loadNextAppointment();
    loadAppointments();
}

// load all appointments in queue order
async function loadQueue() {
    const r    = await fetch(API + '/appointments/queue');
    const data = await r.json();
    // render in priority order
    renderAppointments(data);
}

//Added this from gemini
// Add these functions to your appointment.js

async function loadQueueData() {
    try {
        // 1. Load the "Next Up" Card
        const nextRes = await fetch(API + '/appointments/next');
        const nextData = await nextRes.json();
        const nextDiv = document.getElementById('next-appointment');

        if (nextData.message) {
            nextDiv.innerHTML = `<div class="empty">${nextData.message}</div>`;
        } else {
            nextDiv.innerHTML = `
                <div class="next-appt-card" style="background: #333; padding: 15px; border-radius: 8px; border-left: 4px solid var(--secondary-color);">
                    <h3 style="color: var(--secondary-color);">${nextData.customerName}</h3>
                    <p>${nextData.date} at ${nextData.time}</p>
                    <p><small>${nextData.serviceType}</small></p>
                    <button class="btn-main" onclick="completeNext()">Finish Current</button>
                </div>`;
        }

        // 2. Load the full Queue list
        const queueRes = await fetch(API + '/appointments/queue');
        const queueData = await queueRes.json();
        const queueList = document.getElementById('queue-list');
        
        queueList.innerHTML = queueData.map((a, index) => `
            <div style="display:flex; justify-content:space-between; padding: 10px; border-bottom: 1px solid #444;">
                <span><strong>#${index + 1}</strong> ${a.customerName}</span>
                <span style="color: #888;">${a.time}</span>
            </div>
        `).join('');

    } catch (e) { console.error("Queue load error", e); }
}

async function completeNext() {
    await fetch(API + '/appointments/next/complete', { method: 'PUT' });
    loadQueueData(); // Refresh queue
    loadAppointments(); // Refresh main table
}

// Update your Initial Load at the bottom of the file
loadAppointments();
loadQueueData();

// Initial Load
loadAppointments();