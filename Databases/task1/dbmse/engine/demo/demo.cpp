// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include "../interface/interface.h"
#include "pselectnode.h"
#include "pjoinnode.h"
#include "pcrossproductnode.h"

BaseTable* GetBaseTable1(){
  BaseTable* bt = new BaseTable();
  bt->relpath = "table1\0";
  ValueType* vt = new ValueType[1];
  vt[0] = VT_INT;
  bt->vtypes = &vt[0];
  bt->nbAttr = 1;
  return bt;
}

BaseTable* GetBaseTable2(){
  BaseTable* bt = new BaseTable();
  bt->relpath = "table2\0";
  ValueType* vt = new ValueType[1];
  vt[0] = VT_INT;
  bt->vtypes = &vt[0];
  bt->nbAttr = 1;
  return bt;
}

void FreeBaseTable(BaseTable* bt){
  delete[] bt->vtypes;
  delete bt;
}


class QueryFactory: 
    public BaseQueryFactory
{
public:
    PGetNextNode* dispatch(LSelectNode &node)
    {
        Predicate* p;
        node.GetNextPredicate(&p);
        return new PSelectNode(node.GetBaseTable(), p);
    }

    PGetNextNode* dispatch(LJoinNode &node)
    {
        auto rres = node.GetRight()->accept(*this);
        auto lres = node.GetLeft()->accept(*this);
        return new PJoinNode(lres, rres);
    }

    PGetNextNode* dispatch(LCrossProductNode &node)
    {
         auto conv_le = node.GetLeft()->accept(*this);
         auto conv_rg = node.GetRight()->accept(*this);
         return new PCrossProductNode(conv_le, conv_rg);
    }
};


void ExecuteQuery(PResultNode* query){
  Value* vals = query->GetValuePos();
  ErrCode ec = query->GetRecord();
  while(ec == EC_OK){
    for (int i = 0; i < query->GetAttrNum(); i++){
      if(vals[i].vtype == VT_INT)
        printf("%i", vals[i].vint);
      else if(vals[i].vtype == VT_STRING)
        printf("%s", vals[i].vstr);
    }
    printf("\n");
    ec = query->GetRecord();
  }

}

int main(){
    auto qfactory = QueryFactory();
  /*************************************************************/
  BaseTable* bt1 = GetBaseTable1();
  LAbstractNode* n1 = new LSelectNode(bt1, NULL);
  printf("Starting demo\n");
  printf("Query1: plain select\n");
  auto q1 = n1->accept(qfactory);
  ExecuteQuery(q1);
  delete q1;
  delete n1;
  /*************************************************************/
  printf("Query2: select with predicate\n");
  BaseTable* bt2 = GetBaseTable2();
  std::vector<Predicate>* p2 = new std::vector<Predicate>();
  char* t2 = new char[1];
  t2[0] = '\0';
  p2->push_back(Predicate(PT_GREATERTHAN, VT_INT, 1, 3, t2));
  LAbstractNode* n2 = new LSelectNode(bt2, p2);
  auto q2 = n2->accept(qfactory);
  ExecuteQuery(q2);
  delete q2;
  delete n2;
  /*************************************************************/
  printf("Query3: two selects (one with predicate) and join\n");
  BaseTable* bt3 = GetBaseTable1();
  BaseTable* bt4 = GetBaseTable2();
  LAbstractNode* n3 = new LSelectNode(bt3, NULL);
  std::vector<Predicate>* p4 = new std::vector<Predicate>();
  char* t4 = new char[1];
  t4[0] = '\0';
  p4->push_back(Predicate(PT_GREATERTHAN, VT_INT, 1, 0, t4));
  LAbstractNode* n4 = new LSelectNode(bt4, p4);
  LAbstractNode* n4r = new LJoinNode(n3, n4, 1, 0);
  auto q4 = n4r->accept(qfactory);
  ExecuteQuery(q4);
  delete q4;
  delete n4r;
  /*************************************************************/
  printf("Query4: cross join\n");
  BaseTable* bt5 = GetBaseTable1();
  BaseTable* bt6 = GetBaseTable2();
  LAbstractNode* tbl_one = new LSelectNode(bt5, NULL);
  LAbstractNode* tbl_two = new LSelectNode(bt6, NULL);
  LAbstractNode* cr_join = new LCrossProductNode(tbl_one, tbl_two);
  LAbstractNode* cr_join2 = new LCrossProductNode(cr_join, tbl_two);
   
  auto q5 = cr_join2->accept(qfactory);
  ExecuteQuery(q5);
  delete q5;
  delete cr_join2;
  /************************************************************/
  printf("Query5: cross join of two joins\n");
  BaseTable* bt7 = GetBaseTable1();
  BaseTable* bt8 = GetBaseTable2();
  BaseTable* bt9 = GetBaseTable1();
  BaseTable* bt10 = GetBaseTable2();
  BaseTable* bt11 = GetBaseTable1();
  LAbstractNode* t_zero = new LSelectNode(bt11, NULL);
  LAbstractNode* t_one = new LSelectNode(bt7, NULL);
  LAbstractNode* t_two = new LSelectNode(bt8, NULL);
  LAbstractNode* t_three = new LSelectNode(bt9, NULL);
  LAbstractNode* t_four = new LSelectNode(bt10, NULL);
  LAbstractNode* join_one = new LJoinNode(t_one, t_two, 1, 1);
  LAbstractNode* join_two = new LJoinNode(t_three, t_four, 1, 1);
  LAbstractNode* cr_join_2 = new LCrossProductNode(join_one, join_two);
  LAbstractNode* cr_join_3 = new LCrossProductNode(cr_join_2, t_zero);
  auto q6 = cr_join_3->accept(qfactory);
  ExecuteQuery(q6);
  delete q6;
  delete cr_join_3; 
}
