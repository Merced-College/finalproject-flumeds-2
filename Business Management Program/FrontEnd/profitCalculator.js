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


function calculateProfit() {
    // 1. Get values from inputs (similar to profitCalculator.h members)
    const amountCharged = parseFloat(document.getElementById('p-charged').value) || 0;
    const supplyCost    = parseFloat(document.getElementById('p-supplies').value) || 0;
    const laborRate     = parseFloat(document.getElementById('p-rate').value) || 0;
    const laborHours    = parseFloat(document.getElementById('p-hours').value) || 0;

    // 2. Perform logic (mirrors profitCalculator.cpp)
    const laborCost = laborHours * laborRate;
    const netProfit = amountCharged - supplyCost - laborCost;
    const isProfitable = netProfit > 0;

    // 3. Update the UI
    document.getElementById('res-charged').textContent = `$${amountCharged.toFixed(2)}`;
    document.getElementById('res-supplies').textContent = `-$${supplyCost.toFixed(2)}`;
    document.getElementById('res-labor').textContent = `-$${laborCost.toFixed(2)}`;
    
    const profitEl = document.getElementById('res-profit');
    profitEl.textContent = `$${netProfit.toFixed(2)}`;
    profitEl.className = isProfitable ? 'val-pos' : 'val-neg';

    // 4. Update status badge (mirrors wasProfitable() logic)
    const badge = document.getElementById('profit-badge');
    if (isProfitable) {
        badge.textContent = "✓ This job is profitable!";
        badge.className = "status-message msg-success";
    } else {
        badge.textContent = "⚠ Warning: This job loses money.";
        badge.className = "status-message msg-fail";
    }

    // Show the results panel
    document.getElementById('results-panel').classList.remove('hidden');
}