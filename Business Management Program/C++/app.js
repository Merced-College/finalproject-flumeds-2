// ── Load all customers from C++ ──────────────────────
async function loadCustomers() {
    const response = await fetch('/api/customers');
    const customers = await response.json();

    // update the stat card
    document.getElementById('totalCustomers').textContent = customers.length;

    // build table rows
    const tbody = document.getElementById('customerBody');
    tbody.innerHTML = '';

    customers.forEach(customer => {
        tbody.innerHTML += ` 
            <tr>
                <td>${customer.id}</td>
                <td>${customer.firstName}</td>
                <td>${customer.lastName}</td>
                <td>${customer.phone}</td>
                <td>
                    <button class="delete-btn"
                        onclick="deleteCustomer(${customer.id})">
                        Delete
                    </button>
                </td>
            </tr>
        `;
    });
}

// ── Add a new customer ───────────────────────────────
document.getElementById('addBtn')
    .addEventListener('click', async function() {

        const firstName = document.getElementById('firstName').value;
        const lastName  = document.getElementById('lastName').value;
        const phone     = document.getElementById('phone').value;

        // make sure fields are not empty
        if (!firstName || !lastName || !phone) {
            alert('Please fill in all fields');
            return;
        }

        // send to C++
        await fetch('/api/customers', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ firstName, lastName, phone })
        });

        // clear the form
        document.getElementById('firstName').value = '';
        document.getElementById('lastName').value  = '';
        document.getElementById('phone').value     = '';

        // show success message
        const msg = document.getElementById('successMsg');
        msg.classList.remove('hidden');
        setTimeout(() => msg.classList.add('hidden'), 2000);

        // reload the table
        loadCustomers();
    });

// ── Delete a customer ────────────────────────────────
async function deleteCustomer(id) {
    await fetch('/api/customers/' + id, {
        method: 'DELETE'
    });

    // reload the table
    loadCustomers();
}

// ── Run when page loads ──────────────────────────────
loadCustomers();