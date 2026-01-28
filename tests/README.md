# Tests

- `run_tests.c` : runner principal (compte les PASS/FAIL et met a jour `tests/results.txt`).
- `run_tests.ps1` : compile + execute sous Windows (MinGW).
- `run_tests.sh` : compile + execute sous Linux/macOS.

Les tests graphiques sont **visuels** : l'exe du jeu est lance, vous le fermez manuellement.
Quand il se ferme, le test est marque `passed, but for arbitrary reason`.
