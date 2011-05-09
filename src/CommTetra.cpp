
#include <vector>

//
//  CommTetra.cpp
//
//
//
//                  2009.09.08
//                  2009.09.08
//                  k.Takeda
#include "CommTetra.h"
using namespace pmw;

// construct & destruct
//
CCommTetra::CCommTetra()
{
    //// prolongation-Comm要素
    //mvProgCommElem.reserve(4);

    // Node rank
    mvNodeRank.resize(4);
    mvEdgeRank.resize(6);
    mvFaceRank.resize(4);

    mvbSend.resize(4);
    mvbRecv.resize(4);
    mvbOther.resize(4);

    mvbNodeIXCheck.resize(4);
    mvbDNodeMarking.resize(4);
    uint i;
    for(i=0; i< 4; i++){
        mvbNodeIXCheck[i]=false;
        mvbDNodeMarking[i]=false;

        mvbSend[i]=false;
        mvbRecv[i]=false;
        mvbOther[i]=false;
    }

    // 頂点別の要素集合
    mvvAggCommElem.resize(4);
    mvvNeibCommElemVert.resize(4);

    // CommMesh内のグローバルIndex
    mvCommNodeIndex.resize(4);
}

CCommTetra::~CCommTetra()
{

}

// debug method :所有しているElementの型が一致するか？
//
bool CCommTetra::isTypeCoincidence()
{
    bool bCoin(false);

    if(mpElement->getType()==ElementType::Tetra) bCoin=true;

    return bCoin;
}






















