#include <sys/socket.h>
#include <features.h>         /* for the glibc version number */
#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>     /* the L2 protocols */
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>   /* The L2 protocols */
#endif

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/bigarray.h>



static void
psoc_raise_error(char *prefix, int fd)
{
  char buf[1024];
  snprintf(buf, sizeof(buf)-1, "psoc[%s]:%s", prefix, strerror(errno));
  buf[sizeof(buf)-1] = '\0';
  if(fd >= 0) close(fd);
  printf("error: %s\n", buf);
  caml_failwith(buf);
}

static int
connect_interface(char *dev, char *mac)
{
  //ifreq has union type field
  struct ifreq ifr;
  struct sockaddr_ll sll;
  struct packet_mreq mr;
  int fd;

  if ((fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
    psoc_raise_error("socket", -1);

  strncpy(ifr.ifr_name, dev, IFNAMSIZ);
  if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
    psoc_raise_error("ifindex", fd);
  else
    sll.sll_ifindex = ifr.ifr_ifindex;

  if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
    psoc_raise_error("ifhwaddr", fd);
  else
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);


  sll.sll_family = AF_PACKET;
  sll.sll_protocol = htons(ETH_P_ALL);
  if (bind(fd, (struct sockaddr *)&sll, sizeof(sll)) < 0)
    psoc_raise_error("bind", fd);

  return fd;
}


CAMLprim value
psoc_open(value devname)
{
  CAMLparam1(devname);
  CAMLlocal2(res, mac_caml);

  char dev[IFNAMSIZ];
  char mac[6], mac_str[18];
  int fd;

  memset(dev, 0, sizeof dev);
  memcpy(dev, String_val(devname), caml_string_length(devname));

  fd = connect_interface(dev, mac);

  char *alpha = "0123456789abcdef";
  for (int i = 0; i < sizeof mac; i++) {
    char c = mac[i];
    mac_str[3*i] = alpha[(c&0xF0)>>4];
    mac_str[3*i+1] = alpha[c&0x0F];
    mac_str[3*i+2] = ':';
  }
  mac_str[17] = 0;

  res = caml_alloc_tuple(2);
  mac_caml = caml_copy_string(mac_str);

  Store_field(res, 0, Val_int(fd));
  Store_field(res, 1, mac_caml);

  CAMLreturn(res);
}
