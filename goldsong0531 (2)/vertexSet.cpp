#include "vertexSet.h"

VertexSet::VertexSet(int idx)
{
    this->idx = idx;
    this->parent = this;
}
    
void VertexSet::unionParent(VertexSet* idx)
{
    VertexSet* p = this->getParent();
    idx = idx->getParent();

    if (p < idx) idx->setParent(p);
    else p->setParent(idx);
}

VertexSet* VertexSet::getParent()
{
    if (this->parent == this)
        return this;
    return this->parent->getParent();
}

void VertexSet::setParent(VertexSet* idx)
{
    this->parent = idx;
}

bool VertexSet::isSamePart(VertexSet* idx)
{
    return (this->getParent() == idx->getParent());
}