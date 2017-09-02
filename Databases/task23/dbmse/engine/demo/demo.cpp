// A toy one-column DBMS, v0.3
// George Chernishev (c) 2016, chernishev@gmail.com
// CHANGELOG:
// 0.3: added:
//      1) implemented print features for physical trees
//      updated:
//      1) new format for data files: third line is sort status now
//      2) improved makefile targets
//      3) some code clean-up
// 0.2: first public release

#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include "../interface/interface.h"
#include "pselectnode.h"
#include "phashjoin.h"
#include "pmergejoin.h"
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

BaseTable* GetBaseTable3(){
  BaseTable* bt = new BaseTable();
  bt->relpath = "table3\0";
  ValueType* vt = new ValueType[1];
  vt[0] = VT_INT;
  bt->vtypes = &vt[0];
  bt->nbAttr = 1;
  return bt;
}

BaseTable* GetBigTable(){
  BaseTable* bt = new BaseTable();
  bt->relpath = "table_big\0";
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

        if (GetCombinedStatus(lres->GetSortednessStatus(), rres->GetSortednessStatus()) != CS_NO)
        {
            return new PMergeJoin(lres, rres);
        }

        
        if (lres->QueryHistogram() <= node.GetMemoryLimit())
        {
            return new PHashJoin(lres, rres);    
        }
        

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
  {
      BaseTable* bt1 = GetBaseTable1();
      LAbstractNode* n1 = new LSelectNode(bt1, NULL);
      printf("Starting demo\n");
      printf("Query1: plain select\n");
      auto q1 = n1->accept(qfactory);
      ExecuteQuery(q1);
      delete q1;
      delete n1;
  }
  /*************************************************************/
  {
      printf("Query2: select with predicate\n");
      BaseTable* bt2 = GetBaseTable2();
      std::vector<Predicate>* p2 = new std::vector<Predicate>();
      char* t2 = new char[1];
      t2[0] = '\0';
      p2->push_back(Predicate(PT_GREATERTHAN, VT_INT, 1, 3, t2));
      LAbstractNode* n2 = new LSelectNode(bt2, p2);
      auto q2 = n2->accept(qfactory);
      q2->Print(0);
      ExecuteQuery(q2);
      delete q2;
      delete n2;
  }
  /*************************************************************/
  {
      printf("Query3: two selects (one with predicate) and join\n");
      BaseTable* bt3 = GetBaseTable1();
      BaseTable* bt4 = GetBaseTable2();
      LAbstractNode* n3 = new LSelectNode(bt3, NULL);
      std::vector<Predicate>* p4 = new std::vector<Predicate>();
      char* t4 = new char[1];
      t4[0] = '\0';
      p4->push_back(Predicate(PT_GREATERTHAN, VT_INT, 1, 0, t4));
      LAbstractNode* n4 = new LSelectNode(bt4, p4);
      LAbstractNode* n4r = new LJoinNode(n3, n4, 1, 0, 255);
      auto q4 = n4r->accept(qfactory);
      q4->Print(0);
      ExecuteQuery(q4);
      delete q4;
      delete n4r;
  }
  /*************************************************************/
  {
      printf("Query4: cross join\n");
      BaseTable* bt5 = GetBaseTable1();
      BaseTable* bt6 = GetBaseTable2();
      BaseTable* bt6_1 = GetBaseTable2();
      LAbstractNode* tbl_one = new LSelectNode(bt5, NULL);
      LAbstractNode* tbl_two = new LSelectNode(bt6, NULL);
      LAbstractNode* tbl_two_1 = new LSelectNode(bt6_1, NULL);
      LAbstractNode* cr_join = new LCrossProductNode(tbl_one, tbl_two);
      LAbstractNode* cr_join2 = new LCrossProductNode(cr_join, tbl_two_1);
       
      auto q5 = cr_join2->accept(qfactory);
      q5->Print(0);
      ExecuteQuery(q5);
      delete q5;
      delete cr_join2;
  }
  /************************************************************/
  {
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
      LAbstractNode* join_one = new LJoinNode(t_one, t_two, 1, 1, 255);
      LAbstractNode* join_two = new LJoinNode(t_three, t_four, 1, 1, 255);
      LAbstractNode* cr_join_2 = new LCrossProductNode(join_one, join_two);
      LAbstractNode* cr_join_3 = new LCrossProductNode(cr_join_2, t_zero);
      auto q6 = cr_join_3->accept(qfactory);
      q6->Print(0);
      ExecuteQuery(q6);
      delete q6;
      delete cr_join_3; 
  }
  /************************************************************/
  {
      printf("Query6: join of select and join\n");
      BaseTable* bt2 = GetBaseTable2();
      BaseTable* bt3 = GetBaseTable3();
      BaseTable* bt1 = GetBaseTable1();
      LAbstractNode* tbl_one = new LSelectNode(bt1, NULL);
      LAbstractNode* tbl_two = new LSelectNode(bt2, NULL);
      LAbstractNode* tbl_three = new LSelectNode(bt3, NULL);
      LAbstractNode* join_node_merge = new LJoinNode(tbl_two, tbl_three, 1, 1, 255);
      LAbstractNode* join_node = new LJoinNode(tbl_one, join_node_merge, 1, 1, 255);

      auto q = join_node->accept(qfactory);
      q->Print(0);
      ExecuteQuery(q);
      delete q;
      delete join_node;
  }
  /************************************************************/
  {
      printf("Query7: join from two selects\n");
      BaseTable* bt2 = GetBaseTable2();
      BaseTable* bt3 = GetBaseTable3();
      std::vector<Predicate>* p2 = new std::vector<Predicate>();
      char* t2 = new char[1];
      t2[0] = '\0';
      p2->push_back(Predicate(PT_GREATERTHAN, VT_INT, 1, 1, t2));
      LAbstractNode* tbl_two = new LSelectNode(bt2, p2);
      LAbstractNode* tbl_three = new LSelectNode(bt3, NULL);
      LAbstractNode* join_node = new LJoinNode(tbl_two, tbl_three, 1, 1, 255);

      auto q = join_node->accept(qfactory);
      q->Print(0);
      ExecuteQuery(q);
      delete q;
      delete join_node;
  }
  /************************************************************/
  {
      BaseTable* bt1 = GetBigTable();
      LAbstractNode* n1 = new LSelectNode(bt1, NULL);
      printf("Query8: select from big table, histogram showcase\n");
      printf("10^4 int elements, uniformly generated from [1, 500]\n\n");
      auto q = n1->accept(qfactory);
      q->Print(0);
      using std::cout;
      using std::endl;
      char* t2 = new char[1];
      t2[0] = '\0';
      char* t3 = new char[1];
      t3[0] = '\0';
      char* t4 = new char[1];
      t4[0] = '\0';
      cout << "Histogram queries:" << endl;
      cout << "\tTotal number of elements: " << q->QueryHistogram() << endl;
      auto pred_1 = Predicate(PT_GREATERTHAN, VT_INT, 1, 100, t2);
      cout << "\tElements greater than 100: " << q->QueryHistogram(&pred_1) << endl;
      auto pred_2 = Predicate(PT_GREATERTHAN, VT_INT, 1, 250, t3);
      cout << "\tElements greater than 250: " << q->QueryHistogram(&pred_2) << endl;
      auto pred_3 = Predicate(PT_EQUALS, VT_INT, 1, 250, t4);
      cout << "\tElements equal to 250: " << q->QueryHistogram(&pred_3) << endl;
      //ExecuteQuery(q);
      delete q;
      delete n1;
  }
}
