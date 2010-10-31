#r @"gachelper.dll"

open gachelper

let getGACFullPath name = 
    let fullPath = ref ""
    match GAC.TryGetFullPath(name, fullPath) with
    | true -> printf "%s found at %s" name !fullPath
    | false -> printf "%s not found" name

do
    getGACFullPath "FSharp.Core"

