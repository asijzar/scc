int 		LSQ_IsIteratorPastRear            (                                LSQ_IteratorT                 iterator) 
                  {
	if             (              iterator        == NULL          ) 
return                                               0;

	LSQ_IterT* iter =                               iterator; 
	return iter                                ->flag ==                        1             ;

	
	
	
	
	
	
	
	
	                     }