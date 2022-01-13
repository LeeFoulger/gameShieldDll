rem make sure this is in the root game directory

if exist halo_online.exe (
    start halo_online.exe --account test_account --sign-in-code test_code --environment test_environment
) else (
    start eldorado.exe --account test_account --sign-in-code test_code --environment test_environment
)