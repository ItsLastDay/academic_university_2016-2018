#!/bin/bash
sed -r '
    1i\<html><body><table border="1">
    s|(.*?),(.*?),(.*+)|<tr><td>\1</td><td>\2</td><td>\3</td></tr>|
    $a\</table></body></html>' table  
