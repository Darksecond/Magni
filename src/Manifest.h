#pragma once

#include "StreamReader.h"

#include <memory>
#include <string>

class Manifest
{
public:
    /**
     * This will get a streamreader for a requested resource.
     * This class wil not do any interpreting of resources, it just handles bytes.
     *
     * \param identifier the identifier of the resource to load.
     * \returns the streamreader for the resource, or null if the resource does not exist.
     */
    virtual std::unique_ptr<StreamReader> read(const std::string& identifier) = 0;
};