#!/bin/sh

passed_arguments=$@
make_jobs=$(($(nproc) + 1))

test_arguments=
function execute_test_arguments {
    local arguments=$1

    local command="cmake .. $test_arguments && make clean && make -j $make_jobs"
    echo $command
    eval $command
    if [ ! $? -eq 0 ]; then
        echo "Failed arguments:"
        echo "  $test_arguments"
        exit 1
    fi
    if [ -z "$NO_TESTS" ]; then
        command="make test"
        echo $command
        eval $command
        if [ ! $? -eq 0 ]; then
            echo "Failed arguments:"
            echo "  $test_arguments"
            exit 2
        fi
    fi
}

test_name=
test_count=
test_features=
test_callback=

function test {
    local current=0
    local feature
    for feature in $test_features; do
        local progress=$(printf "test \"$test_name\", combination %u / %u, progress %05.2f%%\n" $(($current + 1)) $test_count $(awk "BEGIN{print $current / $test_count * 100}"))
        echo -ne "\033]2;$progress\007"
        current=$(($current + 1))

        test_arguments="$passed_arguments "
        $test_callback $feature
        test_arguments+="-DTRALLOC_MAN=0 -DTRALLOC_COLLECT_SOURCES=1"
        
        execute_test_arguments
    done
}

function get_arguments_for_extensions {
    local feature=$1
    
    if [ "${feature:0:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_FILE=1 "
    else
        test_arguments+="-DTRALLOC_FILE=0 "
    fi
    if [ "${feature:1:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_THREADS=1 "
    else
        test_arguments+="-DTRALLOC_THREADS=0 "
    fi
    if [ "${feature:2:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_LENGTH=1 "
    else
        test_arguments+="-DTRALLOC_LENGTH=0 "
    fi
    if [ "${feature:3:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DESTRUCTORS=1 "
    else
        test_arguments+="-DTRALLOC_DESTRUCTORS=0 "
    fi
    if [ "${feature:4:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_REFERENCES=1 "
    else
        test_arguments+="-DTRALLOC_REFERENCES=0 "
    fi
    if [ "${feature:5:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_POOL=1 "
    else
        test_arguments+="-DTRALLOC_POOL=0 "
    fi
    if [ "${feature:6:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_UTILS_BUFFER=1 "
    else
        test_arguments+="-DTRALLOC_UTILS_BUFFER=0 "
    fi
}
function test_extensions {
    test_name="extensions"
    test_count=$((2 ** 7))
    test_features=$(echo {0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1})
    test_callback=get_arguments_for_extensions
    test
}

function get_arguments_for_debug {
    local feature=$1

    if [ "${feature:0:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_THREADS=1 "
    else
        test_arguments+="-DTRALLOC_DEBUG_THREADS=0 "
    fi
    if [ "${feature:1:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_CALLBACKS=1 "
    else
        test_arguments+="-DTRALLOC_DEBUG_CALLBACKS=0 "
    fi
    if [ "${feature:2:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_STATS=1 "
    else
        test_arguments+="-DTRALLOC_DEBUG_STATS=0 "
    fi
    if [ "${feature:3:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_LOG=1 "
    else
        test_arguments+="-DTRALLOC_DEBUG_LOG=0 "
    fi
}
function test_debug {
    test_name="debug"
    test_count=$((2 ** 4))
    test_features=$(echo {0..1}{0..1}{0..1}{0..1})
    test_callback=get_arguments_for_debug
    test
}

function get_arguments_for_locks {
    local feature=$1

    if [ "${feature:0:1}" == "2" ]; then
        test_arguments+="-DTRALLOC_SUBTREE_LOCK_TYPE=\"rwlock\" "
    elif [ "${feature:0:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_SUBTREE_LOCK_TYPE=\"mutex\" "
    else
        test_arguments+="-DTRALLOC_SUBTREE_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${feature:1:1}" == "2" ]; then
        test_arguments+="-DTRALLOC_CHILDREN_LOCK_TYPE=\"rwlock\" "
    elif [ "${feature:1:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_CHILDREN_LOCK_TYPE=\"mutex\" "
    else
        test_arguments+="-DTRALLOC_CHILDREN_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${feature:2:1}" == "2" ]; then
        test_arguments+="-DTRALLOC_DEBUG_STATS_LOCK_TYPE=\"rwlock\" "
    elif [ "${feature:2:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_STATS_LOCK_TYPE=\"mutex\" "
    else
        test_arguments+="-DTRALLOC_DEBUG_STATS_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${feature:3:1}" == "2" ]; then
        test_arguments+="-DTRALLOC_DEBUG_THREADS_LOCK_TYPE=\"rwlock\" "
    elif [ "${feature:3:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_THREADS_LOCK_TYPE=\"mutex\" "
    else
        test_arguments+="-DTRALLOC_DEBUG_THREADS_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${feature:4:1}" == "2" ]; then
        test_arguments+="-DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE=\"rwlock\" "
    elif [ "${feature:4:1}" == "1" ]; then
        test_arguments+="-DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE=\"mutex\" "
    else
        test_arguments+="-DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE=\"spinlock\" "
    fi
}
function test_locks {
    test_name="locks"
    test_count=$((3 ** 5))
    test_features=$(echo {0..2}{0..2}{0..2}{0..2}{0..2})
    test_callback=get_arguments_for_locks
    test
}

function get_arguments_for_all_combinations {
    local feature=$1
    
    local extensions_feature=${feature:0:6}
    get_arguments_for_extensions $extensions_feature
    
    local debug_feature=${feature:6:4}
    get_arguments_for_debug $debug_feature
    
    local locks_feature=${feature:10:5}
    get_arguments_for_locks $locks_feature
}
function test_all_combinations {
    test_name="all combinations"
    test_count=$(((2 ** 11) * (3 ** 5)))
    test_features=$(echo {0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..2}{0..2}{0..2}{0..2}{0..2})
    test_callback=get_arguments_for_all_combinations
    test
}

if [ -z "$ALL_COMBINATIONS" ]; then
    test_extensions
    test_debug
    test_locks
else
    test_all_combinations
fi

echo "Success"
