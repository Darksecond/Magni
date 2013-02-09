#import <Foundation/Foundation.h>

#include "DirectoryManifest.h"

const std::string DirectoryManifest::identifierToFile(const std::string& fileName) const
{
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}