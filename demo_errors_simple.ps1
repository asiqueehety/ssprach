#!/usr/bin/env powershell
# SPRACH Compiler - Error Handling Demo

Write-Host "===== SPRACH Error Handling Demonstration =====" -ForegroundColor Cyan
Write-Host ""

# Test 1: Division by Zero
Write-Host "Test 1: Division by Zero" -ForegroundColor Yellow
Write-Host "Code: geben 10 / 0;" -ForegroundColor White
"geben 10 / 0;" | ./sprach 2>&1 | Select-String "ERROR|^[0-9]" | Write-Host -ForegroundColor Green
Write-Host ""

# Test 2: Undefined Variable
Write-Host "Test 2: Undefined Variable" -ForegroundColor Yellow
Write-Host "Code: geben undefined_var;" -ForegroundColor White
"geben undefined_var;" | ./sprach 2>&1 | Select-String "ERROR" | Write-Host -ForegroundColor Green
Write-Host ""

# Test 3: Math Domain Error
Write-Host "Test 3: Math Domain Error" -ForegroundColor Yellow
Write-Host "Code: geben quadrat_wurzel(-4);" -ForegroundColor White
"geben quadrat_wurzel(-4);" | ./sprach 2>&1 | Select-String "ERROR" | Write-Host -ForegroundColor Green
Write-Host ""

# Test 4: Undefined Function
Write-Host "Test 4: Undefined Function" -ForegroundColor Yellow
Write-Host "Code: geben my_function(5);" -ForegroundColor White
"geben my_function(5);" | ./sprach 2>&1 | Select-String "ERROR" | Write-Host -ForegroundColor Green
Write-Host ""

# Test 5: Valid Code (No Error)
Write-Host "Test 5: Valid Code (Should Work)" -ForegroundColor Yellow
Write-Host "Code: GANZ x = 10; geben x;" -ForegroundColor White
"GANZ x = 10; geben x;" | ./sprach 2>&1 | Select-String "ERROR"
if ($? -eq $false) {
    Write-Host "OK - No errors detected" -ForegroundColor Green
}
Write-Host ""

Write-Host "===== All tests completed =====" -ForegroundColor Cyan
