# SPRACH Compiler - Error Handling Demonstration
# This PowerShell script demonstrates various error cases

Write-Host "╔════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║   SPRACH Compiler - Runtime Error Handling Demonstration      ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

# Ensure sprach is compiled
Write-Host "Compiling SPRACH compiler..." -ForegroundColor Yellow
gcc -o sprach sprach.c -lm 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    exit 1
}
Write-Host "✓ Compilation successful" -ForegroundColor Green
Write-Host ""

# Function to run test and display results
function Test-Error {
    param(
        [string]$Name,
        [string]$Code,
        [string]$Description
    )
    
    Write-Host "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" -ForegroundColor Cyan
    Write-Host "Test: $Name" -ForegroundColor Yellow
    Write-Host "Description: $Description" -ForegroundColor White
    Write-Host ""
    Write-Host "Code:" -ForegroundColor White
    Write-Host "  $Code" -ForegroundColor Gray
    Write-Host ""
    
    $output = $Code | ./sprach 2>&1
    
    # Extract and display errors
    $errors = $output | Select-String "ERROR"
    
    if ($errors) {
        Write-Host "Errors Detected:" -ForegroundColor Green
        $errors | ForEach-Object {
            Write-Host "  $_" -ForegroundColor Red
        }
    } else {
        Write-Host "No errors detected" -ForegroundColor Yellow
    }
    
    # Show output (last lines)
    Write-Host ""
    Write-Host "Output:" -ForegroundColor White
    $lastLines = $output | Select-Object -Last 5
    $lastLines | ForEach-Object {
        Write-Host "  $_" -ForegroundColor Gray
    }
    Write-Host ""
}

# Test 1: Undefined Variable
Test-Error `
    -Name "Undefined Variable" `
    -Code "geben undefined_var;" `
    -Description "Accessing a variable that was never declared"

# Test 2: Division by Zero
Test-Error `
    -Name "Division by Zero" `
    -Code "geben 10 / 0;" `
    -Description "Attempting to divide a number by zero"

# Test 3: Modulo by Zero
Test-Error `
    -Name "Modulo by Zero" `
    -Code "geben 17 % 0;" `
    -Description "Attempting to compute modulo with zero divisor"

# Test 4: Square Root of Negative
Test-Error `
    -Name "Square Root of Negative" `
    -Code "geben quadrat_wurzel(-4);" `
    -Description "Taking square root of a negative number"

# Test 5: Log of Zero
Test-Error `
    -Name "Logarithm of Zero" `
    -Code "geben mathe_log(0);" `
    -Description "Taking logarithm of zero (undefined)"

# Test 6: Log of Negative
Test-Error `
    -Name "Logarithm of Negative" `
    -Code "geben mathe_log(-5);" `
    -Description "Taking logarithm of negative number (undefined)"

# Test 7: Undefined Function
Test-Error `
    -Name "Undefined Function" `
    -Code "geben my_function(5);" `
    -Description "Calling a function that doesn't exist"

# Test 8: Array Out of Bounds
Test-Error `
    -Name "Array Out of Bounds" `
    -Code "GANZ arr[] = {1, 2, 3}; geben arr[100];" `
    -Description "Accessing array element beyond array size"

# Test 9: Undefined Array
Test-Error `
    -Name "Undefined Array" `
    -Code "geben undefined_array[0];" `
    -Description "Accessing an array that was never declared"

# Test 10: Valid Operation (No Error)
Write-Host "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" -ForegroundColor Cyan
Write-Host "Test: Valid Operation (No Error)" -ForegroundColor Yellow
Write-Host "Description: Correct code that should execute without errors" -ForegroundColor White
Write-Host ""
Write-Host "Code:" -ForegroundColor White
Write-Host "  GANZ x = 10; GANZ y = 3; geben x + y;" -ForegroundColor Gray
Write-Host ""

$output = "GANZ x = 10; GANZ y = 3; geben x + y;" | ./sprach 2>&1
$errors = $output | Select-String "ERROR"

if ($errors) {
    Write-Host "Errors Detected:" -ForegroundColor Red
    $errors | ForEach-Object {
        Write-Host "  $_" -ForegroundColor Red
    }
} else {
    Write-Host "✓ No errors detected - execution successful!" -ForegroundColor Green
}

Write-Host ""
Write-Host "Result:" -ForegroundColor White
$result = $output | Select-String "13"
if ($result) {
    Write-Host "  ✓ Correct result: 13" -ForegroundColor Green
}

Write-Host ""
Write-Host "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" -ForegroundColor Cyan

# Summary
Write-Host ""
Write-Host "╔════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║                    DEMONSTRATION COMPLETE                      ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan

Write-Host ""
Write-Host "✓ Error Handling Features Demonstrated:" -ForegroundColor Green
Write-Host "  ✓ Undefined variable detection" -ForegroundColor Green
Write-Host "  ✓ Division by zero detection" -ForegroundColor Green
Write-Host "  ✓ Modulo by zero detection" -ForegroundColor Green
Write-Host "  ✓ Math domain error detection" -ForegroundColor Green
Write-Host "  ✓ Function existence checking" -ForegroundColor Green
Write-Host "  ✓ Array bounds checking" -ForegroundColor Green
Write-Host "  ✓ Variable existence checking" -ForegroundColor Green
Write-Host ""
Write-Host "For more details, see: ERROR_HANDLING.md" -ForegroundColor Yellow
Write-Host ""
