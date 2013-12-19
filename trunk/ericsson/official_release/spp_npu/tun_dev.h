#ifndef _TUN_DEV_H
#define _TUN_DEV_H

int tun_open(char *dev); 
int tap_open(char *dev) ;
int tun_close(int fd, char *dev) ;
int tap_close(int fd, char *dev) ;
int tun_write(int fd, char *buf, int len); 
int tap_write(int fd, char *buf, int len) ;
int tun_read(int fd, char *buf, int len); 
int tap_read(int fd, char *buf, int len) ;
int tap_init(void);
int tap_socket_open(void);
int tap_socket_getarp(unsigned int ipaddr, char* buf) ;  
int tap_socket_send( unsigned int ipaddr,unsigned short port, unsigned int ipaddr_src,unsigned int port_src,unsigned char* buf, int len);
int tap_socket_close(void);
int tap_packet_write(char* buf, int len);
int tap_socket_sethwaddr( unsigned char* mac, char* ifname);
int tap_socket_gethwaddr(unsigned char* mac); 
void set_ifname(char * ifname);
char* get_ifname(void);
void remove_tapcfg(void);
#endif /*_TUN_DEV_H*/
