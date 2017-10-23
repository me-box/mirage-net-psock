### Mirage-net-psock
This is an alternate mirage network device to use when building unikerels in unix.

#### Difference comparing to mirage-net-unix
Inside mirage-net-unix, a network device is created by opening a tap device, whereas in mirage-net-psock, by calling sockets API and opening a raw socket, one would get a network device directly sending to/receving from a network interface.

#### Why you may want to use this
For the obvious, if you want to access traffic directly from network interfaces. It's possbile to do the same with standard mirage-net-unix, however, usually it involves extra local host network configuration: setting up linux bridges, connecting interfaces, and probably setting up NAT rules using iptables utilities. But with mirage-net-psock, calling [`connect`](https://github.com/me-box/mirage-net-psock/blob/master/lib/netif.mli#L22) with the intended interface name in your code will just do you the same. As it fulfills the `ETHIF` signature, you could easily build you own network stacks over it and be ready to talk to the outside world.

#### Use case inside Databox project
This tool lays the ground for [me-box/core-network](https://github.com/me-box/core-network) component, which is responsible for managing the communications between different containers within Databox. After Databox is started, containers are created/removed dynamically, so do the dedicated networks that they are attached to. `core-network` will always have an interface on these networks so that it could sit in the middle and control/monitor the traffic flows. `mirage-net-psock` facilitates the reaction to all the interface-up/-down events for `core-network`.
