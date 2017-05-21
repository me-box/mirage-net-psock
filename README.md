### Mirage-net-psock
A mirage network device alternative for usage targetting unix.

Instead of plugging into a tap device, this implementation opens [a packet socket](../master/lib/netif_stubs.c#L49) providing the interface's name.

#### How to use:
```
git clone https://github.com/sevenEng/mirage-net-psock.git
cd mirage-net-psock
opam pin -y add mirage-net-unix .
```
Then in the unikernel's config file, provide the network interface's name to [`netif`](https://github.com/mirage/mirage/blob/master/lib/mirage.mli) to get a working network device.
