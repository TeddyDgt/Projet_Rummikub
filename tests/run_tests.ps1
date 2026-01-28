$ErrorActionPreference = "Stop"

Write-Host "Building tests..."
$src = @(
  "run_tests.c",
  "..\\src\\Combinaisons.c",
  "..\\src\\Players.c"
)

gcc -std=c11 -Wextra -Wall -I..\\src -o run_tests.exe @src

Write-Host "Running tests..."
./run_tests.exe
