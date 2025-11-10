if [ "$2" != "" ]; then
    ($1) >"$2" 2>&1
    exit_code=$?
    cat "$2"
    exit $exit_code
else
    ($1) 2>&1
fi
