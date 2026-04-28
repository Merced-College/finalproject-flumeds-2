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


//Js for employees page
const API = 'http://localhost:8080/api';
let employees = [];
let sortAsc = true;

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

function nudge(id, d) {
  const el = document.getElementById(id);
  const v = parseFloat(el.value) || 0;
  el.value = parseFloat((v + d).toFixed(2));
}

function esc(s) {
  return String(s)
    .replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;')
    .replace(/"/g,'&quot;');
}

async function loadEmployees() {
  try {
    const r = await fetch(API + '/employees');
    employees = await r.json();
    renderStats();
    renderTable();
    populateSelect();
    populateFilter();
  } catch(e) {
    document.getElementById('table-wrap').innerHTML =
      '<div class="empty">Cannot connect to server at localhost:8080.<br>Make sure your C++ backend is running.</div>';
  }
}

function renderStats() {
  const n   = employees.length;
  const ft  = employees.filter(e => e.isFullTime).length;
  const pay = employees.reduce((s, e) => s + e.totalPay, 0);
  const ah  = n ? employees.reduce((s, e) => s + e.hoursWorked, 0) / n : 0;
  const ar  = n ? employees.reduce((s, e) => s + e.hourlyRate, 0) / n : 0;

  document.getElementById('s-total').textContent   = n;
  document.getElementById('s-ft').textContent      = ft + ' full-time';
  document.getElementById('s-payroll').textContent = '$' + Math.round(pay).toLocaleString('en-US');
  document.getElementById('s-avghrs').textContent  = ah.toFixed(1);
  document.getElementById('s-avgrate').textContent = '$' + ar.toFixed(2);
}

function populateSelect() {
  document.getElementById('log-id').innerHTML = employees.length
    ? employees.map(e => `<option value="${e.id}">${e.id}</option>`).join('')
    : '<option>—</option>';
}

function populateFilter() {
  const types = [...new Set(employees.map(e => e.jobType))].sort();
  const sel = document.getElementById('filter-type');
  const cur = sel.value;
  sel.innerHTML = '<option value="">All types</option>' +
    types.map(t => `<option value="${t}"${t===cur?' selected':''}>${t}</option>`).join('');
}

function renderTable() {
  const f    = document.getElementById('filter-type').value;
  const list = f ? employees.filter(e => e.jobType === f) : [...employees];
  const max  = Math.max(...list.map(e => e.hoursWorked), 1);

  if (!list.length) {
    document.getElementById('table-wrap').innerHTML = '<div class="empty">No employees.</div>';
    return;
  }

  const rows = list.map(e => {
    const bw = Math.round((e.hoursWorked / max) * 56);
    return `<tr>
      <td>
        <div class="td-emp">
          ${avHtml(e.name, 34)}
          <div>
            <div class="td-name">${esc(e.name)}</div>
            <div class="td-phone">${esc(e.phone)}</div>
          </div>
        </div>
      </td>
      <td style="color:#888">${esc(e.jobType)}</td>
      <td>$${e.hourlyRate.toFixed(2)}/hr</td>
      <td>
        <div style="font-weight:500;color:#d0d0d0">${e.hoursWorked} hrs</div>
        <div class="hrs-bar" style="width:${bw}px"></div>
      </td>
      <td class="pay">$${Math.round(e.totalPay).toLocaleString('en-US')}</td>
      <td><span class="badge ${e.isFullTime ? 'badge-ft' : 'badge-pt'}">${e.isFullTime ? 'Full-time' : 'Part-time'}</span></td>
      <td><button class="remove-btn" onclick="removeEmp(${e.id},'${esc(e.name)}')">Remove</button></td>
    </tr>`;
  }).join('');

  document.getElementById('table-wrap').innerHTML = `
    <table>
      <thead><tr>
        <th>Employee</th><th>Job type</th><th>Rate</th>
        <th>Hours</th><th>Total pay</th><th>Status</th><th></th>
      </tr></thead>
      <tbody>${rows}</tbody>
    </table>`;
}

function sortByName() {
  sortAsc = !sortAsc;
  employees.sort((a, b) => sortAsc
    ? a.name.localeCompare(b.name)
    : b.name.localeCompare(a.name));
  renderTable();
}

async function addEmployee() {
  const name  = document.getElementById('inp-name').value.trim();
  const job   = document.getElementById('inp-job').value.trim();
  const rate  = parseFloat(document.getElementById('inp-rate').value);
  const phone = document.getElementById('inp-phone').value.trim();
  const email = document.getElementById('inp-email').value.trim();
  if (!name || !job || isNaN(rate) || !phone) {
    alert('Please fill in name, job type, hourly rate and phone.'); return;
  }
  try {
    const r = await fetch(API + '/employees', {
      method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({name, jobType:job, hourlyRate:rate, phone, email})
    });
    const d = await r.json();
    if (d.success) {
      ['inp-name','inp-job','inp-rate','inp-phone','inp-email']
        .forEach(id => document.getElementById(id).value = '');
      loadEmployees();
    }
  } catch(e) { alert('Server error.'); }
}

async function logHours() {
  const id  = parseInt(document.getElementById('log-id').value);
  const hrs = parseFloat(document.getElementById('log-hrs').value);
  if (!id || isNaN(hrs) || hrs <= 0) return;
  try {
    const r = await fetch(API + '/employees/hours/' + id, {
      method:'PUT', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({hours: hrs})
    });
    const d = await r.json();
    if (d.success) {
      const el = document.getElementById('log-result');
      el.innerHTML = `Hours added — total: ${d.hoursWorked} hrs → pay: <span class="green">$${Number(d.totalPay).toLocaleString('en-US',{minimumFractionDigits:2})}</span>`;
      el.classList.add('show');
      loadEmployees();
    }
  } catch(e) { alert('Server error.'); }
}

async function doSearch() {
  const q   = document.getElementById('search-inp').value.trim();
  const box = document.getElementById('search-results');
  if (!q) { box.innerHTML = ''; return; }
  try {
    const r = await fetch(API + '/employees/search/' + encodeURIComponent(q));
    const d = await r.json();
    if (d.found === false || !d.id) {
      box.innerHTML = `<div style="font-size:12px;color:#555;padding:6px 0">No results.</div>`;
      return;
    }
    box.innerHTML = `
      <div class="emp-row-result">
        ${avHtml(d.name, 34)}
        <div class="emp-inf">
          <div class="ename">${esc(d.name)}</div>
          <div class="emeta">${esc(d.jobType)} · $${d.hourlyRate}/hr · ${d.hoursWorked} hrs</div>
        </div>
        <span class="badge ${d.isFullTime?'badge-ft':'badge-pt'}">${d.isFullTime?'Full-time':'Part-time'}</span>
      </div>`;
  } catch(e) { box.innerHTML='<div style="color:#e74c3c;font-size:12px">Server error.</div>'; }
}

async function removeEmp(id, name) {
  if (!confirm('Remove ' + name + '?')) return;
  try {
    await fetch(API + '/employees/' + id, {method:'DELETE'});
    loadEmployees();
  } catch(e) { alert('Server error.'); }
}

loadEmployees();