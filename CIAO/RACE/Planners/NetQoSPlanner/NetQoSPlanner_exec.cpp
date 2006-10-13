// $Id$
//
// ****              Code generated by the                 ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#include "NetQoSPlanner_exec.h"
#include "ciao/CIAO_common.h"
#include "ciao/NetQoSC.h"
#include <iostream>
#include <fstream>
#include "ciao/DeploymentS.h"
#include "BandwidthBroker/BandwidthBrokerC.h"
//#include "tao/CORBALOC_Parser.h"
#include "ace/INET_Addr.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"

using namespace mil::darpa::arms::mlrm;
using namespace mil::darpa::arms::mlrm::BandwidthBroker;

/*
Named loops idiom
http://www.csse.monash.edu.au/~damian/Idioms/Topics/07.SB.NamedLoops/html/text.html
*/
#define NAMED(blockname) goto blockname; \
 blockname##_skip: if (0) blockname:
#define BREAK(blockname) goto blockname##_skip;

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_NetQoSPlanner_Impl
    {
      //==================================================================
      // Facet Executor Implementation Class:   Planner_I_exec_i
      //==================================================================

      Planner_I_exec_i::Planner_I_exec_i (NetQoSPlanner_exec_i & net_qos)
        : net_qos_planner_exec_(&net_qos)
      {
      }

      Planner_I_exec_i::~Planner_I_exec_i (void)
      {
      }

      // Operations from ::CIAO::RACE::Planner_I

      ::CORBA::Boolean
      Planner_I_exec_i::process_plan (
        ::CIAO::RACE::Plan_Actions &  plans
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CIAO::RACE::PlannerFailure))
      {
        return this->net_qos_planner_exec_->process_plan (plans);
      }
    
      ::CORBA::Boolean
      Planner_I_exec_i::process_domain_change (
        const ::CIAO::RACE::Planner_I::Domain_Changes & /* changes */,
        ::CIAO::RACE::Plan_Actions_out /* plans */
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                      ::CIAO::RACE::PlannerFailure))
      {
        // Your code here.
        return true;
      }

      char *
      Planner_I_exec_i::name (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        return this->net_qos_planner_exec_->name ();
      }
      
      char *
      Planner_I_exec_i::type (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        return this->net_qos_planner_exec_->type ();
      }

      //==================================================================
      // Component Executor Implementation Class:   NetQoSPlanner_exec_i
      //==================================================================

      NetQoSPlanner_exec_i::NetQoSPlanner_exec_i (void)
       : node_map_filename_ ("NodeDetails.dat"),
         planner_name_ ("NetQoSPlanner"),
         planner_type_ ("Netqork QoS Planner")
      {
      }

      NetQoSPlanner_exec_i::~NetQoSPlanner_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      char *
      NetQoSPlanner_exec_i::name (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        ACE_DEBUG ((LM_DEBUG, "(%N:%l): NetQoSPlanner_exec_i::name()\n"));
        return CORBA::string_dup (this->planner_name_.c_str());
      }

      void
      NetQoSPlanner_exec_i::name (
        const char *  name 
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        this->planner_name_ = name;// Your code here.
      }

      char *
      NetQoSPlanner_exec_i::type (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        ACE_DEBUG ((LM_DEBUG, "(%N:%l): NetQoSPlanner_exec_i::type()\n"));
        return CORBA::string_dup (this->planner_type_.c_str());
      }

      void
      NetQoSPlanner_exec_i::type (
        const char *  type 
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        this->planner_type_ = type;
      }

      char *
      NetQoSPlanner_exec_i::node_map_file (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        return CORBA::string_dup(this->node_map_filename_.c_str());
      }

      void
      NetQoSPlanner_exec_i::node_map_file (
        const char *  node_map_file 
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        this->node_map_filename_ = node_map_file;
      }

      // Port operations.

      ::CORBA::Boolean
      NetQoSPlanner_exec_i::process_plan (
        ::CIAO::RACE::Plan_Actions &  plans
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::CIAO::RACE::PlannerFailure))
      {
        bool retval = true;
        for (size_t i = 0; i < plans.length (); ++i)
          {
            ::Deployment::DeploymentPlan &dep_plan = plans[i].plan;
            bool flag_netqos_present = false;

            for (size_t j = 0;
                 j < dep_plan.infoProperty.length();
                 ++j)
              {
                if (ACE_OS::strcmp (dep_plan.infoProperty[j].name.in (),
                                    "CIAONetworkQoS") == 0)
                 {
                   flag_netqos_present = true;
                   ::Deployment::DiffservInfos dscp_infos;
                   ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement *net_qos_req;

                   if (dep_plan.infoProperty [j].value >>= net_qos_req)
                    {
                        this->build_instance_node_map (dep_plan);
                        if (this->process_netqos_req (net_qos_req, dep_plan, dscp_infos))
                          {
                            // Remove CIAONetworkQoS infoProperty
                            CORBA::ULong length = dep_plan.infoProperty.length();
                            ACE_DEBUG ((LM_ERROR, "(%N:%l): Length of dep_plan.infoProperty before removal = %u\n",length));
                            if (length > j+1)
                            {
                              for (size_t k = j + 1; k < length; ++k)
                                {
                                  dep_plan.infoProperty[k-1] = dep_plan.infoProperty[k];
                                }
                            }
                            dep_plan.infoProperty.length(length - 1);
                            // Removal code ends
                            this->add_network_priorities (dep_plan, dscp_infos);
                            retval = true;
                          }
                        else
                          {
                            ACE_DEBUG ((LM_ERROR, "(%N:%l): process_netqos_req failed.\n"));
                            retval = false;
                          }
                    }
                    else
                    {
                      ACE_DEBUG ((LM_ERROR, "(%N:%l)Conversion to Any failed for NetworkQoS.\n"));
                      retval = false;
                    }
                 }
               }
             if (!flag_netqos_present)
               {
                 ACE_DEBUG ((LM_DEBUG, "No CIANetworkQoS info-property defined in this deployment plan.\n"));
               }
          }
        return retval;
      }

      void NetQoSPlanner_exec_i::build_instance_node_map (Deployment::DeploymentPlan & dep_plan)
      {
        for (size_t i = 0;i < dep_plan.instance.length (); ++i)
          {
            Deployment::InstanceDeploymentDescription &instance = dep_plan.instance[i];
            this->instance_node_map_.insert (std::make_pair (instance.name.in (),instance.node.in()));
          }
      }

      bool NetQoSPlanner_exec_i::process_netqos_req (const ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement *net_qos_req,
                                                     const ::Deployment::DeploymentPlan &dep_plan,
                                                     ::Deployment::DiffservInfos & dscp_infos)
      {
        // Build an in memory map of logical nodes to the physical hosts.
        this->build_node_map ();

        if (! this->resolve_BB ())
          {
            ACE_DEBUG ((LM_ERROR, "(%N:%l): Can't contact BandwidthBroker.\n"));
            return false;
          }

        bool rollback = false;
        size_t set_len = net_qos_req->conn_qos_set.length();
        //ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: set_len = %u\n",set_len));

        NAMED(outer)
        for (size_t k = 0; k < set_len; ++k)
          {
            const ::CIAO::DAnCE::NetworkQoS::ConnectionQoS & conn_qos = net_qos_req->conn_qos_set[k];
            //ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: conn_qos.connections.length () = %u\n",conn_qos.connections.length ()));
            for (size_t conn_num = 0;
                 conn_num < conn_qos.connection_names.length ();
                 ++conn_num)
              {
                ::Deployment::DiffservInfo endpoint;
                if (this->get_endpoints(endpoint, dep_plan, std::string (conn_qos.connection_names[conn_num])))
                {
                  CommonDef::IPAddress srcIP;
                  if (-1 == this->find_ip_address (srcIP, endpoint.client_instance_name.in()))
                  {
                    ACE_DEBUG ((LM_ERROR,"In NetQoSPlanner_exec_i::process_netqos_req: Can't find source IP\n"));
                    rollback = true;
                    BREAK(outer);
                  }
                  ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: srcIP = %s\n",srcIP.dottedDecimal.in()));
                  CommonDef::IPAddress destIP;
                  if (-1 == this->find_ip_address(destIP, endpoint.server_instance_name.in()))
                  {
                    ACE_DEBUG ((LM_ERROR,"In NetQoSPlanner_exec_i::process_netqos_req: Can't find destination IP\n"));
                    rollback = true;
                    BREAK(outer);
                  }
                  ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: destIP = %s\n",destIP.dottedDecimal.in()));
                  CommonDef::QOSRequired qos_req;
                  this->get_traffic_qos (qos_req, conn_qos);
                  long fwd_dscp = 0, rev_dscp = 0;

                  if (-1 == this->make_flow_request (srcIP, destIP, conn_qos.fwdBWD, qos_req, fwd_dscp))
                  {
                    rollback = true;
                    BREAK(outer);
                  }

                  size_t len = dscp_infos.length ();
                  dscp_infos.length (len + 1);
                  dscp_infos [len].server_instance_name = endpoint.server_instance_name;
                  dscp_infos [len].client_instance_name = endpoint.client_instance_name;;
                  //dscp_infos [len].client_receptacle_name = CORBA::string_dup (conn_qos.connections [conn_num].client_port_name);
                  dscp_infos [len].request_dscp = fwd_dscp;
                  dscp_infos [len].reply_dscp = rev_dscp; /// Assigning zero here.
                  ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: fwd_dscp = %d\n",fwd_dscp));

                  if (conn_qos.revBWD > 0)
                  {
                    if (-1 == this->make_flow_request (destIP, srcIP, conn_qos.revBWD, qos_req, rev_dscp))
                    {
                      rollback = true;
                      BREAK(outer);
                    }
                    ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: rev_dscp = %d\n",rev_dscp));
                    dscp_infos [len].reply_dscp = rev_dscp;
                  }
                  /*              std::cerr
                                  << "Connection Name = " << conn_qos.connections [conn_num].connection_name   << std::endl
                                  << "client Name = " << conn_qos.connections [conn_num].client                << std::endl
                                  << "client Port Name = " << conn_qos.connections [conn_num].client_port_name << std::endl
                                  << "server Name = " << conn_qos.connections [conn_num].server                << std::endl
                                  << "server Port Name = " << conn_qos.connections [conn_num].server_port_name << std::endl;
                   */
                }
              else
                {
                  ACE_DEBUG((LM_DEBUG,"Connection was not found in the deployment plan.\n"));
                }  
                /*
                   std::cerr << "fwdBWD = " << conn_qos.fwdBWD << std::endl;
                   std::cerr << "revBWD = " << conn_qos.revBWD << std::endl;

                   if (::CIAO::DAnCE::NetworkQoS::NORMAL == conn_qos.priority)
                   std::cerr << "Priority = NORMAL\n";
                   else if (::CIAO::DAnCE::NetworkQoS::HIGH == conn_qos.priority)
                   std::cerr << "Priority = HIGH\n";
                   else if (::CIAO::DAnCE::NetworkQoS::LOW == conn_qos.priority)
                   std::cerr << "Priority = LOW\n";
                 */
              }
          }

          if (rollback)
            {
              this->BB_proxy_.rollback ();
              return false;
            }
          else
            {
              this->BB_proxy_.commit ();
              return true;
            }
      }

      bool NetQoSPlanner_exec_i::get_endpoints (::Deployment::DiffservInfo &diffserv_conn, 
                                                const ::Deployment::DeploymentPlan &dep_plan,
                                                const std::string &conn_name)
      {
        size_t total_connections = dep_plan.connection.length();
        for (size_t num_conn = 0; num_conn < total_connections; ++num_conn)
          {
            if (conn_name == dep_plan.connection[num_conn].name.in())
              {
                const ::Deployment::PlanConnectionDescription & conn_desc =
                      dep_plan.connection [num_conn];
                unsigned long index = conn_desc.internalEndpoint[0].instanceRef;
                diffserv_conn.server_instance_name = dep_plan.instance[index].name;
                index = conn_desc.internalEndpoint[1].instanceRef;
                diffserv_conn.client_instance_name = dep_plan.instance[index].name;
                //diffserv_conn.client_receptacle_name = 
                return true;
              } 
          }
        return false;
      }

      int NetQoSPlanner_exec_i::find_ip_address (CommonDef::IPAddress & ip, const char *instance_name)
      {
        const int FAKE_PORT = 20000;
        std::string inst_name (instance_name);
        if (this->instance_node_map_.find (inst_name) != this->instance_node_map_.end ())
        {
          const std::string &logical_node = this->instance_node_map_[inst_name];
          ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: logical_node = %s\n",logical_node.c_str()));
          //const std::string &physical_node = this->get_physical_host (logical_node);
          ip.dottedDecimal = CORBA::string_dup (logical_node.c_str());
          ip.subnetMask = CORBA::string_dup ("255.255.255.255");
          return 0;
/*          ACE_INET_Addr addr (FAKE_PORT, physical_node.c_str (), AF_INET);
          if (const char *ip_addr = addr.get_host_addr ())
            {
              ip.dottedDecimal = CORBA::string_dup (ip_addr);
              ip.subnetMask = CORBA::string_dup ("255.255.255.255");
              return 0;
            }
          else
            return -1;
*/        }
        else
          return -1;
      }

      void
      NetQoSPlanner_exec_i::get_traffic_qos (CommonDef::QOSRequired &qos_req,
                                             const ::CIAO::DAnCE::NetworkQoS::ConnectionQoS & conn_qos)
      {
          if (::CIAO::DAnCE::NetworkQoS::HIGH_PRIORITY == conn_qos.data_qos)
            qos_req = CommonDef::highPriority;
          else if (::CIAO::DAnCE::NetworkQoS::HIGH_RELIABILITY == conn_qos.data_qos)
            qos_req = CommonDef::highReliability;
          else if (::CIAO::DAnCE::NetworkQoS::VIDEO == conn_qos.data_qos)
            qos_req = CommonDef::mmVideo;
          else if (::CIAO::DAnCE::NetworkQoS::VOICE == conn_qos.data_qos)
            qos_req = CommonDef::mmVoice;
          else if (::CIAO::DAnCE::NetworkQoS::BEST_EFFORT == conn_qos.data_qos)
            qos_req = CommonDef::bestEffort;
          /*
          else if (::CIAO::DAnCE::NetworkQoS::HIGH_THROUGHPUT == conn_qos.data_qos)
            qos_req = CommonDef::highThroughput;
          */
          else
            qos_req = CommonDef::raw;
      }

      int NetQoSPlanner_exec_i::make_flow_request (const CommonDef::IPAddress &srcIP,
                                                   const CommonDef::IPAddress &destIP,
                                                   int bandwidth,
                                                   CommonDef::QOSRequired qos_req,
                                                   long &dscp)
      {
          AdmissionControl::FlowInfo flowinfo;

          flowinfo.srcIP = srcIP;
          flowinfo.srcPort.low = -1;
          flowinfo.srcPort.high = -1;

          flowinfo.destIP = destIP;
          flowinfo.destPort.low = -1;
          flowinfo.destPort.high = -1;

          flowinfo.protocol = AdmissionControl::notSpecified;
          flowinfo.fwdRate.requiredBW = CORBA::Long (bandwidth);
          flowinfo.fwdRate.extraBW = CORBA::Long (0);
          flowinfo.revRate.requiredBW = CORBA::Long (0);
          flowinfo.revRate.extraBW = CORBA::Long (0);
          flowinfo.biDirectional = CORBA::Boolean (false);
          flowinfo.flowDuration = CORBA::Long (1000);

          ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: Requesting flow.\n"));
          int result = this->BB_proxy_.flow_request (flowinfo, qos_req, dscp);

          if (-1 == result)
              ACE_DEBUG ((LM_ERROR,"In NetQoSPlanner_exec_i::process_netqos_req: Requested flow was not admitted.\n"));
          else
              ACE_DEBUG ((LM_DEBUG,"In NetQoSPlanner_exec_i::process_netqos_req: Flow Accepted.\n"));

          return result;
      }

      void NetQoSPlanner_exec_i::build_node_map ()
      { 
          //std::cerr << "Node_map Filename: " << this->node_map_file () << std::endl;
          std::ifstream input_file (this->node_map_file ());
          std::istream_iterator <std::string> begin (input_file), end;
          for (; begin != end; )
            {
               std::string logical_node = *begin++;
               if (begin == end)
                break;
               //std::cerr << "Logical node name = " << logical_node << std::endl;
               std::string corbaloc_url = *begin++;
               //std::cerr << "Physical corbaloc URL = " << corbaloc_url << std::endl;
               //TAO_CORBALOC_Parser corbaloc_parser;
               //std:: cerr << "Is it a valid corbaloc URL? = " << corbaloc_parser.match_prefix (corbaloc_url.c_str()) << std::endl;
               if (1/*corbaloc_parser.match_prefix (corbaloc_url.c_str())*/)
                 {
                    size_t first = corbaloc_url.find (':', 0); 
                    size_t second = corbaloc_url.find (':', first + 1);
                    size_t third = corbaloc_url.find (':', second + 1);
                    std::string hostname = corbaloc_url.substr (second + 1, third - second - 1);
                    //std::cerr << "Hostname = " << hostname << std::endl;
                    this->node_map_.insert (make_pair (logical_node, hostname));
                 }
               else
                 {
                    ACE_DEBUG ((LM_ERROR, "In NetQoSPlanner_exec_i::process_netqos_req(): Incorrect corbaloc URL"));
                 }
            }
      }

      std::string NetQoSPlanner_exec_i::get_physical_host (const std::string &logical_node)
      {
        if (this->node_map_.find (logical_node) != this->node_map_.end ())
        {
           return this->node_map_[logical_node];
        }
        else
           return std::string ("");
      }

      // *********************************************************
      // code that creates the deployment plan populating the network
      // priority policies using the diffserv codepoint decisions.

      void
      NetQoSPlanner_exec_i::add_network_priorities (Deployment::DeploymentPlan & temp_plan,
                                                    const Deployment::DiffservInfos & dscp_infos)
      {
        //Deployment::DeploymentPlan temp_plan = plan.in ();

        //Deployment::DiffservInfos dscp_infos;
        //Deployment::DiffservInfo dscp_info;

        //int request_array_slot = 3;
        //int reply_array_slot = 2;
/*
        dscp_info.server_instance_name =
          CORBA::string_dup ("Hello-Sender-idd");
        dscp_info.client_instance_name =
          CORBA::string_dup ("Hello-Receiver-idd");
        dscp_info.client_receptacle_name =
          CORBA::string_dup ("read_message");
        dscp_info.request_dscp = dscp[request_array_slot];
        dscp_info.reply_dscp = dscp[reply_array_slot];

        CORBA::ULong dscp_infos_len = dscp_infos.length ();
        CORBA::ULong new_dscp_infos_len;
        new_dscp_infos_len = dscp_infos_len + 1;
        dscp_infos.length (new_dscp_infos_len);
        dscp_infos[dscp_infos_len] = dscp_info;
*/
        CORBA::ULong len = dscp_infos.length ();
        CIAO::DAnCE::ServerResource server_resource;
        server_resource.Id =
          CORBA::string_dup ("test_server_resource_id");

        for (CORBA::ULong i = 0; i < len; ++i)
          {
            const char *server_instance_name =
                dscp_infos[i].server_instance_name.in ();
            const char *client_instance_name =
                dscp_infos[i].client_instance_name.in ();
            const char *client_receptacle_name =
                dscp_infos[i].client_receptacle_name.in ();
            CORBA::Long request_dscp = dscp_infos[i].request_dscp;
            CORBA::Long reply_dscp = dscp_infos[i].reply_dscp;

            CORBA::ULong instance_len = temp_plan.instance.length ();
            for (CORBA::ULong j = 0; j < instance_len; ++j)
              {
                const char *instance_name =
                  temp_plan.instance[j].name.in ();
                this->instance_map_.bind (instance_name, j);
              }

            CORBA::ULong policy_set_len;
            CORBA::ULong new_policy_set_len;
            CORBA::ULong policy_def_len;
            CORBA::ULong new_policy_def_len;
            CORBA::ULong dep_res_len;
            CORBA::ULong new_dep_res_len;
            CORBA::ULong pro_len;
            CORBA::ULong new_pro_len;
            ACE_CString policy_set_id;
            ACE_CString temp_policy_set_id;

            int server_instance_iter = 0;
            if (this->instance_map_.find
                    (server_instance_name, server_instance_iter) == 0)
              {
                policy_set_len =
                  server_resource.orb_config.policy_set.length ();
                new_policy_set_len = policy_set_len + 1;
                server_resource.orb_config.policy_set.length (
                    new_policy_set_len);
                policy_set_id = "test_policy_set_id";
                policy_set_id += "_";
                policy_set_id += server_instance_name;
                server_resource.orb_config.policy_set[policy_set_len].Id =
                  CORBA::string_dup (policy_set_id.c_str ());
    
                policy_def_len = server_resource.orb_config.
                                    policy_set[policy_set_len].
                                      policies.length ();
                new_policy_def_len = policy_def_len + 1;
                server_resource.orb_config.policy_set[policy_set_len].
                  policies.length (new_policy_def_len);
    
                ::CIAO::DAnCE::NWPriorityModelPolicyDef npmd;
                npmd.nw_priority_model =
                  ::CIAO::DAnCE::CLIENT_PROPAGATED_NWPRIORITY;
                npmd.request_dscp = 0;
                npmd.reply_dscp = 0;
    
                server_resource.orb_config.policy_set[policy_set_len].
                  policies[policy_def_len].NWPriorityModelDef (npmd);
    
                dep_res_len =
                  temp_plan.instance[server_instance_iter].
                    deployedResource.length ();
                new_dep_res_len = dep_res_len + 1;
                temp_plan.instance[server_instance_iter].
                    deployedResource.length (new_dep_res_len);
                temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].resourceUsage =
                      Deployment::InstanceUsesResource;
                temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].requirementName =
                    CORBA::string_dup ("CIAO:PolicySet");
                temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].resourceName =
                    CORBA::string_dup ("test_server_resource_id");
                pro_len = temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].property.length ();
                new_pro_len = pro_len + 1;
                temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].property.length (
                    new_pro_len);
                temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].property[pro_len].
                      name = CORBA::string_dup ("CIAO:InstancePolicy");
                temp_plan.instance[server_instance_iter].
                  deployedResource[dep_res_len].property[pro_len].
                      value <<= policy_set_id.c_str ();
              }
    
            int client_instance_iter = 0;
            if (this->instance_map_.find
                    (client_instance_name, client_instance_iter) == 0)
              {
                policy_set_len =
                  server_resource.orb_config.policy_set.length ();
                new_policy_set_len = policy_set_len + 1;
                server_resource.orb_config.policy_set.length (
                    new_policy_set_len);
    
                policy_set_id = "test_policy_set_id";
                policy_set_id += "_";
                policy_set_id += client_instance_name;
                temp_policy_set_id = "test_policy_set_id";
                temp_policy_set_id += "_";
                temp_policy_set_id += server_instance_name;
    
                server_resource.orb_config.policy_set[policy_set_len].Id =
                  CORBA::string_dup (policy_set_id.c_str ());
    
                policy_def_len = server_resource.orb_config.
                                    policy_set[policy_set_len].
                                      policies.length ();
                new_policy_def_len = policy_def_len + 1;
                server_resource.orb_config.policy_set[policy_set_len].
                  policies.length (new_policy_def_len);
    
                ::CIAO::DAnCE::CNWPriorityModelPolicyDef cnpmd;
                cnpmd.request_dscp = request_dscp;
                cnpmd.reply_dscp = reply_dscp;
    
                server_resource.orb_config.policy_set[policy_set_len].
                  policies[policy_def_len].CNWPriorityModelDef (cnpmd);
    
                dep_res_len =
                  temp_plan.instance[client_instance_iter].
                    deployedResource.length ();
                new_dep_res_len = dep_res_len + 1;
                temp_plan.instance[client_instance_iter].
                    deployedResource.length (new_dep_res_len);
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].resourceUsage =
                      Deployment::InstanceUsesResource;
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].requirementName =
                    CORBA::string_dup ("CIAO:PolicySet");
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].resourceName =
                    CORBA::string_dup ("test_server_resource_id");
    
                pro_len = temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property.length ();
                new_pro_len = pro_len + 1;
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property.length (
                    new_pro_len);
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property[pro_len].
                      name = CORBA::string_dup ("CIAO:ReceptaclePolicy");
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property[pro_len].
                      value <<= policy_set_id.c_str ();
    
                pro_len = temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property.length ();
                new_pro_len = pro_len + 1;
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property.length (
                    new_pro_len);
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property[pro_len].
                      name = client_receptacle_name;
                temp_plan.instance[client_instance_iter].
                  deployedResource[dep_res_len].property[pro_len].
                      value <<= policy_set_id.c_str ();
              }
          }
    
        CORBA::ULong new_info_prop_len;
        CORBA::ULong info_prop_len = temp_plan.infoProperty.length ();
        new_info_prop_len = info_prop_len + 1;
        temp_plan.infoProperty.length (new_info_prop_len);
        temp_plan.infoProperty[new_info_prop_len - 1].name = CORBA::string_dup ("CIAOServerResources");
        temp_plan.infoProperty[new_info_prop_len - 1].value <<= server_resource;
      }

      bool
      NetQoSPlanner_exec_i::resolve_BB ()
      {
        return this->BB_proxy_.resolve (this->context_->_ciao_the_Container ()->the_ORB ());
      }

      ::CIAO::RACE::CCM_Planner_I_ptr
      NetQoSPlanner_exec_i::get_planner_i (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException))
      {
        // Your code here.
        return new Planner_I_exec_i (*this);
        //return ::CIAO::RACE::CCM_Planner_I::_nil ();
      }

      // Operations from Components::SessionComponent

      void
      NetQoSPlanner_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        this->context_ =
          NetQoSPlanner_Context::_narrow (
            ctx
            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW ( ::CORBA::INTERNAL ());
        }
      }

      void
      NetQoSPlanner_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      NetQoSPlanner_exec_i::ciao_postactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      NetQoSPlanner_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      NetQoSPlanner_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      void
      NetQoSPlanner_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   NetQoSPlanner_Home_exec_i
      //==================================================================

      NetQoSPlanner_Home_exec_i::NetQoSPlanner_Home_exec_i (void)
      {
      }

      NetQoSPlanner_Home_exec_i::~NetQoSPlanner_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      NetQoSPlanner_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC (( ::CORBA::SystemException,
                       ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
          ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          NetQoSPlanner_exec_i,
          ::CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" NETQOSPLANNER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_NetQoSPlanner_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
          ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          NetQoSPlanner_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}

