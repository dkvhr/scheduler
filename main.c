#include <stdio.h>
#include <stdlib.h>
#include"types.h"

int main(void) {
        NodeHead *high_priority_queue = create_node_head();
        NodeHead *low_priority_queue = create_node_head();
        NodeHead *IO_queue = create_node_head();
        NodeHead *procs_queue = create_node_head();

        ProcessIO *disk1 = create_IO_proc(0, 0);
        //Process *P1 = create_process(1, 0, 10, 0, 1);

        return 0;
}