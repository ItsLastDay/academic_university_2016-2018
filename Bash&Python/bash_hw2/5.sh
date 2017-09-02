#!/bin/bash
last_status=0
req_string="https://api.vk.com/method/users.get?user_ids=347745&fields=online&v=5.8"

while (( 1 ))
do
    sleep 60
    api_ans=$(wget "$req_string" -q -O - | grep -oP "\"online\":\d")
    cur_status=${api_ans: -1}

    if [[ $cur_status != $last_status ]]
    then
        if [[ $cur_status == 0 ]]
        then
            cat <<< "AM goes offline" | wall
        else
            cat <<< "AM goes online" | wall
        fi
        last_status=$cur_status
    fi
done
