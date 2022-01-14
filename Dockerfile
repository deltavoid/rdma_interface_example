FROM deltavoid/debian_cpp_basic:2022.01.13


ADD ./ /repos/rdma_interface_example


RUN cd /repos/rdma_interface_example && make build
