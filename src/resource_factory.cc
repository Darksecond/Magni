#include "resource_factory.h"

resource_factory::resource_factory()
{
}

resource_factory& resource_factory::instance()
{
    static resource_factory _instance;
    return _instance;
}

void resource_factory::add_manifest(std::shared_ptr<Ymir::Manifest> manifest)
{
    _manifests.add(manifest);
}

void resource_factory::add_loader(const std::string& type, std::shared_ptr<resource_loader> loader)
{
    _loaders.insert(std::make_pair(type, loader));
}
