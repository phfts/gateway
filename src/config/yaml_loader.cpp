#pragma once

#include <string>

#include "../utils/string.cpp"
#include "../../lib/wsjcpp-yaml/src/wsjcpp_yaml.cpp"

WsjcppYaml loadYaml(std::string file)
{
    WsjcppYaml yaml;
    std::string sYaml, sError;

    sYaml = readFileToString(file);
    if (!yaml.loadFromString(file, sYaml, sError))
    {
        yaml.throw_err("MAIN", sError);
        throw std::runtime_error("Invalid file " + file);
    }
    return yaml;
}