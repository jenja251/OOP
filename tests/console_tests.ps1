param([string]$Executable = "$PSScriptRoot/../build/bakery.exe")
$ErrorActionPreference = 'Stop'
$OutputEncoding = [System.Text.UTF8Encoding]::new($false)
[Console]::OutputEncoding = $OutputEncoding
$checks = 0

function Run-Case([string]$InputText, [string[]]$Expected, [string[]]$Arguments = @()) {
    $output = ($InputText | & $Executable @Arguments) -join "`n"
    if ($LASTEXITCODE -ne 0) { throw "Program failed: $LASTEXITCODE" }
    foreach ($text in $Expected) {
        if (-not $output.Contains($text)) { throw "Missing output: $text" }
        $script:checks++
    }
}

# Create bread with decimal comma, add it, remove from catalog, change cart quantity, remove row.
Run-Case "2`n1`nCustomBread`n100`nRye`n0,5`n4`n4`n2`n3`n4`n6`n1`n3`n5`n7`n1`n5`n0" @(
    'Товар создан.', 'CustomBread', 'Товар добавлен в корзину.',
    'Товар удалён из каталога.', 'Количество изменено.', 'Итого: 150.00',
    'Строка удалена из корзины.', 'Корзина пуста.', 'До свидания!'
)
# Create pastry and cake, use spaces in names, verify price formulas via receipt.
Run-Case "2`n2`nCustom Pastry`n65`nCherry`n15`n2`n3`nCustom Cake`n300`n3`nBerries`n200`n4`n4`n2`n4`n5`n1`n5`n0" @(
    'Custom Pastry', 'Custom Cake', 'Итого: 1260.00'
)
# Invalid input returns to menu; rejected changes preserve an existing valid order.
Run-Case "abc`n1x`n99`n4`n99`n4`n1`n0`n4`n1`n2`n6`n1`n-1`n5`n7`n0`n2`n1`nBadBread`n-10`nRye`n1`n0" @(
    'Введите корректное число.', 'После числа не должно быть других символов.',
    'Выберите действие от 0 до 7.', 'Товара с таким номером нет.',
    'Количество товара должно быть больше нуля.', 'Итого: 120.00',
    'Базовая цена должна быть конечным неотрицательным числом.'
)
# Empty catalog, empty cart, and EOF in the middle of product creation.
Run-Case "3`n1`n3`n1`n3`n1`n1`n4`n5`n6`n7`n2`n1" @(
    'Каталог пуст.', 'Корзина пуста.', 'Ввод завершён.'
)
Run-Case '' @('Итого: 1460.00', 'Итого: 1380.00', 'Корзина пуста.') @('--demo')
Write-Output "PASS: $checks console checks"
