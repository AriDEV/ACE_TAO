// -*- C++ -*-
// $Id$

// SSL_SOCK_Acceptor.i

#if defined (ACE_HAS_SSL)

// Do nothing routine for constructor.

ASYS_INLINE
ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}

ASYS_INLINE
ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
                                              int reuse_addr,
                                              int protocol_family,
                                              int backlog,
                                              int protocol)
  : acceptor_ (local_sap,
               reuse_addr,
               protocol_family,
               backlog,
               protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}

ASYS_INLINE
ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
                                              ACE_Protocol_Info *protocolinfo,
                                              ACE_SOCK_GROUP g,
                                              u_long flags,
                                              int reuse_addr,
                                              int protocol_family,
                                              int backlog,
                                              int protocol)
  : acceptor_ (local_sap,
               protocolinfo,
               g,
               flags,
               reuse_addr,
               protocol_family,
               backlog,
               protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}

ASYS_INLINE int
ACE_SSL_SOCK_Acceptor::open (const ACE_Addr &local_sap,
                             int reuse_addr,
                             int protocol_family,
                             int backlog,
                             int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::open");
  if (this->acceptor_.open (local_sap,
                            reuse_addr,
                            protocol_family,
                            backlog,
                            protocol) != 0)
    return -1;
  else
    this->set_handle (this->acceptor_.get_handle ());

  return 0;
}

ASYS_INLINE int
ACE_SSL_SOCK_Acceptor::close (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::close ()");
  return this->acceptor_.close ();
}

ASYS_INLINE
ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor");
}


#endif /* ACE_HAS_SSL */
