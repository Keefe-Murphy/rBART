//#######################################################################################
//put this as a header file!
//#######################################################################################

#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::export]]
  IntegerVector find_term_child(NumericMatrix tree_table){
  NumericVector node_status=tree_table.column(4);
  IntegerVector internal_nodes_prop;
  
  for(int i=0;i<node_status.size();i++){
   // std::cout<<"i is"<<" "<<i<<"node status is"<<node_status[i]<<"\n";
    if(node_status[i]==1){
    //std::cout<<"NODE STATUS=1!!"<<"\n";
      internal_nodes_prop.push_back(i+1);
      
    }
  }
  
  //std::cout<<"Internal_nodes_prop"<<internal_nodes_prop[1]<<" "<<internal_nodes_prop.size()<<"\n";
  
  IntegerVector both_child_term(internal_nodes_prop.size());
 // std::cout<<"Internal_nodes_prop"<<internal_nodes_prop.size();
  /*for(int  i = 0; i < internal_nodes_prop.size(); ++i){
   std::cout <<"in for loop"<< internal_nodes_prop[i] << ' ';
  }*/
  for(int k=0;k<internal_nodes_prop.size();k++){
   // std::cout<<"Row index is"<<" "<<k<<" "<<tree_table(internal_nodes_prop[k]-1,1);
    //std::cout<<"tree index is"<<" "<<k<<" "<<tree_table(tree_table(internal_nodes_prop[k]-1,1)-1,4)<<"\n";
   // std::cout<<"tree index is"<<tree_table(tree_table(internal_nodes_prop[k]-1,1),4)<<"\n";
  // std::cout<<"logical test"<<(tree_table(tree_table(internal_nodes_prop[k]-1,0),4)==-1)<<"\n";
    if((tree_table(tree_table(internal_nodes_prop[k]-1,0)-1,4)==
    tree_table(tree_table(internal_nodes_prop[k]-1,1)-1,4)) && (tree_table(tree_table(internal_nodes_prop[k]-1,0),4)==-1)){
      //std::cout<<"Both children are terminal! \n";
      both_child_term[k]=1;
    
    }else{
      both_child_term[k]=0;
    }
  }
//  std::cout<<"both_child_term"<<both_child_term[3]<<"\n";
  IntegerVector nodes_to_prune;
  
  for(int k=0;k<both_child_term.size();k++){
    if(both_child_term[k]==1){
      nodes_to_prune.push_back(internal_nodes_prop[k]);     
    }
  }  
 // std::cout<<"both_child_term"<<both_child_term[3]<<"\n";
  return(nodes_to_prune);
}
    
//#######################################################################################  
#include <Rcpp.h>
//#include "find_term_child.h"
using namespace Rcpp;
// [[Rcpp::export]]
double choose_prune(double ratio,double tree_ratio,NumericMatrix prior_tree,NumericMatrix proposal_tree){
  double ratio_out;
  double var_density=1;
  double q_t_tstar;
  double q_tstar_t;
  double qratio;
  
  NumericVector terminal_nodes_pro;
  IntegerVector nodes_to_prune=find_term_child(prior_tree);
 // std::cout<<"nodes to prune is"<<nodes_to_prune[1];
  if(prior_tree.nrow()==1){
    q_t_tstar=1;
   // std::cout<<"nrow=1 \n";
  }else{
    double node_size=nodes_to_prune.size();
    double num_nodes=1/node_size;
   // std::cout<<"nodes size is"<<node_size<<"num_nodes is "<<num_nodes<<"\n";
    q_t_tstar=0.25*num_nodes;
    //std::cout<<"Tree is not a stump!"<<nodes_to_prune.size()<<" "<<q_t_tstar<<"\n";
  }
  
  //std::cout<<"qttstar is"<<q_t_tstar<<"\n";
  //get terminal_nodes
  for(int l=0;l<proposal_tree.nrow();l++){
    
    if(proposal_tree(l,4)==-1){
      
      terminal_nodes_pro.push_back(l+1);
    //  std::cout<<"i is"<<l<<"\n";
      
    }
  }
  //for(int i=0;i<terminal_nodes_pro.size();i++)
  //  std::cout<<"terminal_nodes are "<<" "<<terminal_nodes_pro[i]<<"\n";
    
  q_tstar_t=0.25*(1.0/terminal_nodes_pro.size())*(var_density);
 // std::cout<<"q_tstar_t"<<q_tstar_t<<"\n";
//  std::cout<<"q_t_tstar"<<q_t_tstar<<"\n";
  qratio=q_tstar_t/q_t_tstar;
//   std::cout<<"qratio"<<qratio<<"\n";
 //  std::cout<<"tree ratio"<<tree_ratio<<"\n";
  ratio_out=ratio*tree_ratio*qratio;
//   std::cout<<"ratio"<<ratio<<"\n";
  return(ratio_out);
}
