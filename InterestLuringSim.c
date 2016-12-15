#include <stdlib.h>
#include <stdio.h>

int main (int ac, char **av)
{
	FILE *input;
	char Buffer1[20];
	char Buffer2[20];	

	typedef struct {
		int ASn;
		int ParentIndex;
		short unsigned int neighbour;
		int Dist;
		unsigned char edge;
		unsigned char bot;
		unsigned char identify;
		unsigned char attacker;
		unsigned char flood;
		int flow;
		int range;
	} Node;

	typedef struct {
		short unsigned int SrcIndex;
		short unsigned int DstIndex;
		int PID;
		unsigned char identify;
		int metric;
	} Link;
	
	Node nodes[51000];
	int NodeIndex = 0;
	Link links[430000];	
	int LinkIndex = 0;
	int PID = 1;

	srand((int)time(NULL));
	
	av++;
	input = fopen(*av, "r");
	if (input == NULL)
	{
		printf("Open the 1st file failed!");
		exit(0);
	}

	while(fscanf(input, "%s %s", Buffer1, Buffer2) != EOF)
	{
		int AS1 = atoi(Buffer1);
		int AS2 = atoi(Buffer2);
		int as1index = -1;
		int as2index = -1;
		
		int i = 0;
		for (i=0; i<NodeIndex; i++){
			if (nodes[i].ASn == AS1 ){
				as1index = i;
				nodes[i].neighbour++;
				break;
			}
		}

		if (i == NodeIndex){
			nodes[NodeIndex].ASn = AS1;
			nodes[NodeIndex].neighbour = 1;
			nodes[NodeIndex].ParentIndex = -1;
			nodes[NodeIndex].Dist = 1000000;
			nodes[NodeIndex].edge = 1;
			nodes[NodeIndex].bot = 0;
			nodes[NodeIndex].identify = 0;
			nodes[NodeIndex].attacker = 0;
			nodes[NodeIndex].flow = 0;
			nodes[NodeIndex].range = 10000;
			nodes[NodeIndex].flood = 0;
			as1index = NodeIndex;
			NodeIndex++;
		}

		for (i=0; i<NodeIndex; i++){
	            if (nodes[i].ASn == AS2 ){
        	        as2index = i;
			nodes[i].neighbour++;
                	break;
             	    }
        	}

        	if (i == NodeIndex){
            		nodes[NodeIndex].ASn = AS2;
			nodes[NodeIndex].neighbour = 1;
			nodes[NodeIndex].ParentIndex = -1;
        		nodes[NodeIndex].Dist = 1000000;
			nodes[NodeIndex].edge = 1;
		        nodes[NodeIndex].bot = 0;
            		nodes[NodeIndex].identify = 0;
					nodes[NodeIndex].attacker = 0;
					nodes[NodeIndex].flow = 0;
					nodes[NodeIndex].range = 10000;
					nodes[NodeIndex].flood = 0;
            		as2index = NodeIndex;
            		NodeIndex++;
        	}
				
		links[LinkIndex].SrcIndex = as1index;
		links[LinkIndex].DstIndex = as2index;
		links[LinkIndex].PID = PID;
		links[LinkIndex].identify = 0;
		links[LinkIndex].metric = 1;
		LinkIndex++;
		links[LinkIndex].SrcIndex = as2index;
		links[LinkIndex].DstIndex = as1index;	
		links[LinkIndex].PID = PID;
		links[LinkIndex].identify = 0;
		links[LinkIndex].metric = 1;
		LinkIndex++;
		PID++;
	}
	
	fclose(input);

	printf("%d\n", LinkIndex);	
	printf("%d\n", NodeIndex);
	printf("Topology learned!\n");	

	av++;
        input = fopen(*av, "r");
        if (input == NULL)
        {
                printf("Open the 2nd file failed!");
                exit(0);
        }

	int numOfCore = 0;
	
	while(fscanf(input, "%s", Buffer1) != EOF)
        {
                int coreAS = atoi(Buffer1);
                		
                int i = 0;
                for (i=0; i<NodeIndex; i++){
                        if (nodes[i].ASn == coreAS ){
                                nodes[i].edge = 0;
				numOfCore++;
                                break;
                        }
                }
        }

	fclose(input);

	av++;
	int attacker_num = atoi(*av);
	printf("Attacker number: %d\n", attacker_num);

	av++;
	int mode_num = atoi(*av);
	printf("Mode number: %d\n", mode_num);

	av++;
	int bot_num = atoi(*av);
	printf("Bot number: %d\n", bot_num);
	
	av++;
	int flood_num = atoi(*av);

	int i;

	for (i=0; i<NodeIndex; i++)
                if (nodes[i].neighbour == 1)
                        nodes[i].edge = 1;


	//	Print the degree of each AS.
	for (i=0; i<NodeIndex; i++)
		if (nodes[i].edge == 0)
			printf("%d %d\n", nodes[i].ASn, nodes[i].neighbour);
	

	int numOfEdge = 0;

	for (i=0; i<NodeIndex; i++)
                if (nodes[i].edge == 1)
                        numOfEdge++;
	
	printf("Edge identified! Total number: %d\n", numOfEdge);
			
	int attackers[attacker_num];	
	int attackerIndex = -1;
	

	for (i=0; i<attacker_num; i++)
	{
		while (1){
			attackerIndex = (int)rand() % NodeIndex;
			if (nodes[attackerIndex].edge == 1 && nodes[attackerIndex].attacker == 0){
				attackers[i] = attackerIndex;
				nodes[attackerIndex].attacker = 1;
				break;
			}
		}
	}
		
	printf("Attacker(s) identified!\n");

	for (i=0; i<attacker_num; i++)
		printf("Attacker %d: %d\n", i+1, nodes[attackers[i]].ASn);


int numOfBot = 0;
int botIndex;

switch (mode_num)
{
	case 1:
//************************  Identify part of edges as bot!  *****************************//	
	
	/* Changeable parameter! */
	while (numOfBot <= bot_num)	{
		
		botIndex = (int)rand() % NodeIndex;		
		if (nodes[botIndex].attacker != 1 && nodes[botIndex].edge == 1 && nodes[botIndex].bot == 0)	{
			nodes[botIndex].bot = 1;
			numOfBot++;
		}
	}
	break;
//****************************************************************************************//

	case 2:
//********************************   All edges!   *****************************************//

	for (i=0; i<NodeIndex; i++)
                if (nodes[i].edge == 1 && nodes[i].attacker != 1)
                        nodes[i].bot = 1;

	break;
//****************************************************************************************//

	case 3:
//************************  Identify part of cores as bot!  *****************************//     
      
      /* Changeable parameter! */
      while (numOfBot <= bot_num)        {
              
              botIndex = (int)rand() % NodeIndex;         
              if (nodes[botIndex].attacker != 1 && nodes[botIndex].edge == 0 && nodes[botIndex].bot == 0) {
                      nodes[botIndex].bot = 1;
                      numOfBot++;
              }
      }
	  break;
//****************************************************************************************//


	case 4:
//********************************   All cores!   *****************************************//

        for (i=0; i<NodeIndex; i++)
                if (nodes[i].edge == 0 && nodes[i].attacker != 1)
                        nodes[i].bot = 1;

	break;
//****************************************************************************************//


	case 5:
//************************  Identify part of all nodes as bot!  *****************************//     
      
      /* Changeable parameter! */
      while (numOfBot <= bot_num)        {
             
              botIndex = (int)rand() % NodeIndex;         
              if (nodes[botIndex].attacker != 1 && nodes[botIndex].bot == 0) {
                      nodes[botIndex].bot = 1;
                      numOfBot++;
              }
      }

	  break;
//****************************************************************************************//	


	case 6:
//********************************   All nodes!   *****************************************//

      for (i=0; i<NodeIndex; i++)
                if (nodes[i].attacker != 1)
                        nodes[i].bot = 1;

	  break;
//****************************************************************************************//

}


	printf("Bot identified!\n");
	
	
int k;	
for (k=0; k<attacker_num ; k++)
{
	
	attackerIndex = attackers[k];
	
	nodes[attackerIndex].Dist = 0;

	
	for (i=1; i<NodeIndex; i++)
    {
        unsigned char mod = 0;

        int j = 0;
		for ( j=0; j<LinkIndex; j++)
        {
            int src = links[j].SrcIndex;
            int dest = links[j].DstIndex;

            if (nodes[src].Dist + links[j].metric >= nodes[dest].Dist)
                continue;

            nodes[dest].Dist = nodes[src].Dist + links[j].metric;
            nodes[dest].ParentIndex = src;

            mod = 1;
        }

        if (mod == 0)
            break;
    }

	printf("Shortest paths for attacker %d computed!\n", k+1);

	/*	Print the parent of each AS. 

	for (i=0; i<NodeIndex; i++)
		printf("%d %d\n", nodes[i].ASn, nodes[i].ParentIndex);
	
	*/

	int pointer;
	
	for (i=0; i<NodeIndex; i++){
		if (nodes[i].bot == 1){
			pointer = i;
			while (nodes[pointer].ParentIndex != -1){
				nodes[pointer].identify = 1;
				int m;
				for (m=0; m<LinkIndex; m++)
					if ((links[m].SrcIndex==pointer && links[m].DstIndex==nodes[pointer].ParentIndex) || (links[m].DstIndex==pointer && links[m].SrcIndex==nodes[pointer].ParentIndex))
						links[m].identify = 1;
				pointer = nodes[pointer].ParentIndex;	
			}
		}
	}

	printf("Identify for attacker %d complete!\n", k+1);
	
	for (i=0; i<NodeIndex; i++)
		if (nodes[i].Dist == 1 && nodes[i].range > 1)
			nodes[i].range = 1;
	
	for (i=0; i<NodeIndex; i++)
		if (nodes[i].Dist == 2 && nodes[i].range > 2)
			nodes[i].range = 2;
	
	for (i=0; i<NodeIndex; i++){
		nodes[i].Dist = 1000000;
		nodes[i].ParentIndex = -1;
	}

}

	int identifiedNodes = 0;
	int identifiedLinks = 0;

	for (i=0; i<NodeIndex; i++)
		if (nodes[i].identify == 1)
			identifiedNodes++;

	for (i=0; i<LinkIndex; i++)
		if (links[i].identify == 1)
			identifiedLinks++;

	printf("%d	", identifiedNodes);
	printf("%d	", identifiedLinks);
	

/********************************   Attacking victim, case 1.   *************************************/


	int victimIndex;

	while (1){
		victimIndex = (int)rand() % NodeIndex;
		if (nodes[victimIndex].bot == 1)
			break;
	}
	
	nodes[victimIndex].Dist = 0;
	
	for (i=1; i<NodeIndex; i++)
    {
        unsigned char mod = 0;

        int j = 0;
		for ( j=0; j<LinkIndex; j++)
        {
            int src = links[j].SrcIndex;
            int dest = links[j].DstIndex;

            if (nodes[src].Dist + links[j].metric >= nodes[dest].Dist)
                continue;

            nodes[dest].Dist = nodes[src].Dist + links[j].metric;
            nodes[dest].ParentIndex = src;

            mod = 1;
        }

        if (mod == 0)
            break;
    }

	printf("Case 1 : shortest paths to the victim computed!\n");
	
	int numOfFlood = 0;
	int floodIndex;

	while (numOfFlood <= flood_num)	{
		
		floodIndex = (int)rand() % NodeIndex;		
		if (nodes[floodIndex].bot == 1 && floodIndex != victimIndex && nodes[floodIndex].flood == 0)	{
			nodes[floodIndex].flood = 1;
			numOfFlood++;
		}
	}

	int pointer;
	
	int numOfEffectivePath = 0;
	
	for (i=0; i<NodeIndex; i++){
		
		if (nodes[i].flood == 1){
			
			int isEffectivePath = 1;
			pointer = i;
			
			while (nodes[pointer].ParentIndex != -1){
				
				int m;
				for (m=0; m<LinkIndex; m++)
					if (links[m].SrcIndex==pointer && links[m].DstIndex==nodes[pointer].ParentIndex)
						isEffectivePath = isEffectivePath * links[m].identify;
							
				nodes[pointer].flow++;
				pointer = nodes[pointer].ParentIndex;	
			}
			
			numOfEffectivePath = numOfEffectivePath + isEffectivePath;
		}
	}

	printf("%d	", numOfEffectivePath);

	int flow_num = 0;
	int flow_sum = 0;
	int flow_max = 0;

	printf("Case 1: attacker flows: \n");

	for (i=0; i<attacker_num; i++){
	
		flow_num++;
		flow_sum = flow_sum + nodes[attackers[i]].flow;
		if (nodes[attackers[i]].flow > flow_max)
			flow_max = nodes[attackers[i]].flow;
	}		
		
	printf("%d		%.2f		%d		", flow_sum, (float)flow_sum/(float)flow_num, flow_max);
	
	flow_num = 0;
	flow_sum = 0;
	flow_max = 0;

	printf("Case 1: range <=1 flows: \n");

	for (i=0; i<NodeIndex; i++)
		if (nodes[i].range <= 1){
			flow_num++;
			flow_sum = flow_sum + nodes[i].flow;
			if (nodes[i].flow > flow_max)
				flow_max = nodes[i].flow;
		}

	printf("%d		%.2f		%d		", flow_sum, (float)flow_sum/(float)flow_num, flow_max);

	flow_num = 0;
	flow_sum = 0;
	flow_max = 0;

	printf("Case 1: range <=2 flows: \n");

	for (i=0; i<NodeIndex; i++)
		if (nodes[i].range <= 2){
			flow_num++;
			flow_sum = flow_sum + nodes[i].flow;
			if (nodes[i].flow > flow_max)
				flow_max = nodes[i].flow;
		}

	printf("number: %d, sum: %d, mean: %.2f, max: %d\n", flow_num, flow_sum, (float)flow_sum/(float)flow_num, flow_max);



/****************************************************************************************************/


/********************************   Attacking victim, case 2.   *************************************/

	
	for (i=0; i<NodeIndex; i++){
		nodes[i].Dist = 1000000;
		nodes[i].ParentIndex = -1;
		nodes[i].flow = 0;
	}

	for (i=0; i<LinkIndex; i++)
		if (nodes[links[i].SrcIndex].range <= 2 || nodes[links[i].DstIndex].range <= 2)
			links[i].metric = 1000;	
			
	for (i=0; i<LinkIndex; i++)
		if (links[i].identify == 0)
			links[i].metric = 1000000;


	nodes[victimIndex].Dist = 0;
	
	for (i=1; i<NodeIndex; i++)
    {
        unsigned char mod = 0;

        int j = 0;
		for ( j=0; j<LinkIndex; j++)
        {
            int src = links[j].SrcIndex;
            int dest = links[j].DstIndex;

            if (nodes[src].Dist + links[j].metric >= nodes[dest].Dist)
                continue;

            nodes[dest].Dist = nodes[src].Dist + links[j].metric;
            nodes[dest].ParentIndex = src;

            mod = 1;
        }

        if (mod == 0)
            break;
    }

	printf("Case 2: shortest paths to the victim computed!\n");

	for (i=0; i<NodeIndex; i++){
		
		if (nodes[i].flood == 1){
						
			pointer = i;
			
			while (nodes[pointer].ParentIndex != -1){
						
				nodes[pointer].flow++;			
				pointer = nodes[pointer].ParentIndex;	
			}			
		}
	}

	
	flow_num = 0;
	flow_sum = 0;
	flow_max = 0;

	printf("Case 2: attacker flows: \n");

	for (i=0; i<attacker_num; i++){
	
		flow_num++;
		flow_sum = flow_sum + nodes[attackers[i]].flow;
		if (nodes[attackers[i]].flow > flow_max)
			flow_max = nodes[attackers[i]].flow;
	}		
		
	printf("%d		%.2f		%d		", flow_sum, (float)flow_sum/(float)flow_num, flow_max);
	
	flow_num = 0;
	flow_sum = 0;
	flow_max = 0;

	printf("Case 2: range <=1 flows: \n");

	for (i=0; i<NodeIndex; i++)
		if (nodes[i].range <= 1){
			flow_num++;
			flow_sum = flow_sum + nodes[i].flow;
			if (nodes[i].flow > flow_max)
				flow_max = nodes[i].flow;
		}

	printf("%d		%.2f		%d\n", flow_sum, (float)flow_sum/(float)flow_num, flow_max);

	flow_num = 0;
	flow_sum = 0;
	flow_max = 0;

	printf("Case 2: range <=2 flows: \n");

	for (i=0; i<NodeIndex; i++)
		if (nodes[i].range <= 2){
			flow_num++;
			flow_sum = flow_sum + nodes[i].flow;
			if (nodes[i].flow > flow_max)
				flow_max = nodes[i].flow;
		}

	printf("number: %d, sum: %d, mean: %.2f, max: %d\n", flow_num, flow_sum, (float)flow_sum/(float)flow_num, flow_max);

/****************************************************************************************************/

	return 0;
}
