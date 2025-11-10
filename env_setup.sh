build()
{
    echo make -f make/Makefile $@
    make -f make/Makefile $@
}
suffix()
{
    suff=${@: -1}
    words=("${@:1:$((($#)-1))}")

    res=""
    for word in $words;do
        res="$res$word$suff "
    done
    echo "$res"
}
compile()
{
    if [ "$#" -eq 0 ];then
        build compile_all
    else
        build $(suffix $1 _compile) ${@:2}
    fi
}
link()
{
    if [ "$#" -eq 0 ];then
        build link_all
    else
        build $(suffix $1 _link) ${@:2}
    fi
}
run()
{
    build $(suffix $1 _run) ARGS="\"${*:2}\""
}