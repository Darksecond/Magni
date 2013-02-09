#import <Foundation/Foundation.h>

#include "DirectoryManifest.h"

using namespace Ymir;

/**
 * This is neccecary for OSX to find files in bundles.
 * Other operating sytems need to provide their own variant.
 */
const std::string DirectoryManifest::identifierToFile(const std::string& fileName) const
{
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}