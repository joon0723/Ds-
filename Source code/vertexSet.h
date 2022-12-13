#ifndef _VERTEXSET_H_
#define _VERTEXSET_H_

class VertexSet{
private:
	VertexSet* parent;
    int idx;

public:
	VertexSet(int idx);
	~VertexSet();	
		
	void unionParent(VertexSet* idx);
	VertexSet* getParent();
    void setParent(VertexSet* idx);
	bool isSamePart(VertexSet* idx);
};

#endif