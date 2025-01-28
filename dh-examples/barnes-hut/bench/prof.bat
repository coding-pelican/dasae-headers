llvm-profdata merge -output="default.profdata" -instr "default.profraw"
llvm-cov show "main.exe" -instr-profile="default.profdata" -format=html -output-dir="default-show"
llvm-cov report "main.exe" -instr-profile="default.profdata"

@REM -fprofile-instr-generate -fcoverage-mapping
