#include "soapH.h"
#include "stdsoap2.h"
#include "SOAP.nsmap"

//#include "net/netapi.h"
//#include "pthread.h"

#include "MyDataSql.h"
#include <sstream>

#if defined (WIN32)
#include <process.h>  
#include <Windows.h>
#endif

#include "ace/Service_Config.h"
#include "ace/Process_Semaphore.h"
#include "ace/OS.h"
#include "ace/Init_ACE.h"

#if !defined (ACE_WIN32)
int ProcessLock(const char *file, pid_t *pid, int fwr)
#endif

#define BACKLOG (100)	// Max. request backlog 
#define MAX_THR (10)	// Max. threads to serve requests 

using namespace std;
//所有new的地方需要改成使用soap_new__，如soap_new___ns1__MeasurementList 否则会造成内存泄露

//static ProcMan proc_manager;

/*#if defined (__unix)
void *Checkproc (void*arg)
#elif defined (WIN32)
UINT  Checkproc (void *arg)
#endif
{
	U32INT check_count=0;
	while(true)
	{
		if((check_count++)%25==0)
		{
			if (!proc_manager.CheckParentProc())
			{
				break;
			}
		}
		_delay_time(40000);
	}
	return NULL;
}*/

int main(int argc, ACE_TCHAR * argv[]) 
{
// 	if ( argc != 1 ) 
// 	{
// 		printf("Usage: %s port/n", argv[0]);
// 		exit(-1);
// 	}

	ACE::init();	

#if defined (ACE_WIN32)
	ACE_Process_Semaphore sem(1, ACE_TEXT("GET_RDB_DATA"));
	if(sem.tryacquire() == -1)
	{
		ACE::fini();
		return 0;
	}
#else
	pid_t pid = 0;
	int lockfd = ProcessLock("netbus.lock", &pid, 1);
	if(lockfd < 0)
	{
		ACE::fini();
		exit(-1);
	}
#endif

	//APLInit();

	int port = 7790;

	struct soap soap;
	soap_init(&soap);
// 	soap_set_mode(&soap,SOAP_C_MBSTRING);
//  	soap.mode |= SOAP_C_UTFSTRING;
	//proc_manager.StartProc("rtbdataget", "实时库数据读取服务进程","");
	//proc_manager.RegProc("实时库数据读取服务进程");

#if defined (__unix)
	for(int aa=1;aa<33;aa++)
	{
		if(aa!=SIGUSR1)
			sigset(aa,SIG_IGN);	
	}

	{
		if(setsid()==(pid_t)-1){
			if (setpgrp()==-1)
				printf("\n can't change process group\n");
			else
				printf("\n change process group ok\n");
		}
		else
			printf("\n change process group ok\n");
	}

	pthread_t		idthread;
	if(pthread_create(&idthread,NULL,Checkproc,0)<0) exit(1); 
#elif defined (WIN32)
	 //_beginthreadex( NULL, 0, Checkproc, NULL , 0, NULL );
#endif

	soap_omode(&soap, SOAP_XML_DEFAULTNS);

	soap.send_timeout = 60; // 60 seconds 
	soap.recv_timeout = 60; // 60 seconds 

	struct soap *soap_thr[MAX_THR]; // each thread needs a runtime context 
	//pthread_t tid[MAX_THR];
	ACE_thread_t threadId[MAX_THR];
	ACE_hthread_t threadHandle;
	SOAP_SOCKET m, s; 
	int i; 
	m = soap_bind(&soap, NULL, port, BACKLOG); 
	if (!soap_valid_socket(m)) 
	{
		//APLExit();
		ACE::fini();

		exit(1);
	}

	fprintf(stderr, "Socket connection successful %d\n", m); 
	for (i = 0; i < MAX_THR; i++) 
		soap_thr[i] = NULL; 
	for (;;) 
	{ 
		for (i = 0; i < MAX_THR; i++) 
		{ 
			s = soap_accept(&soap); 
			if (!soap_valid_socket(s)) 
			{ 
				if (soap.errnum) 
				{ 
					soap_print_fault(&soap, stderr); 
					continue; // retry
				} 
				else
				{ 
					fprintf(stderr, "Server timed out\n"); 
					break; 
				} 
			} 
			fprintf(stderr, "Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", i, s, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF); 
			if (!soap_thr[i]) // first time around 
			{ 
				soap_thr[i] = soap_copy(&soap); 
				if (!soap_thr[i]) 
				{
					//APLExit();
					ACE::fini();

					exit(1); // could not allocate 
				}
			} 
			else// recycle soap context 
			{ 
				//pthread_join(tid[i], NULL);
				ACE_Thread::join(threadHandle);
				fprintf(stderr, "Thread %d completed\n", i); 
				soap_destroy(soap_thr[i]); // deallocate C++ data of old thread 
				soap_end(soap_thr[i]); // deallocate data of old thread 
			} 
			soap_thr[i]->socket = s; // new socket fd 
			//pthread_create(&tid[i], NULL, (void*(*)(void*))soap_serve, (void*)soap_thr[i]);
			ACE_Thread::spawn(
				(ACE_THR_FUNC)soap_serve,        //线程执行函数
				(void*)soap_thr[i],							 //执行函数参数
				THR_JOINABLE | THR_NEW_LWP,
				&threadId[i],
				&threadHandle
				);
		} 
	} 
	for (i = 0; i < MAX_THR; i++) {
		if (soap_thr[i]) 
		{ 
			soap_done(soap_thr[i]); // detach context 
			free(soap_thr[i]); // free up 
		} 
	}

	//proc_manager.UnRegProc();
	//proc_manager.EndProc();

	//APLExit();
	ACE::fini();

	return 0; 

	/*int m, s;
	if ((m = soap_bind(&soap, NULL, port, 100)) < 0) 
	{
		soap_print_fault(&soap, stderr);
	}
	else 
	{
		printf("Socket connect successfully: master socket = %d\n", m);
		int i = 0;
		while ( 1 ) 
		{
			if ( (s = soap_accept(&soap)) < 0 ) 
			{
				soap_print_fault(&soap, stderr);
				soap_end(&soap);
				continue;
			}

			printf("Connection %d accepted from IP = %d.%d.%d.%d, slave socket = %d\n", ++i, (soap.ip >> 24) & 0xff, (soap.ip >> 16) & 0xff, (soap.ip >> 8) & 0xff, soap.ip & 0xff, s);
			if ( soap_serve(&soap) != SOAP_OK ) 
			{
				soap_print_fault(&soap, stderr);
				soap_end(&soap);
				continue;
			}
			soap_end(&soap);
		}
	}
	soap_done(&soap);
	proc_manager.UnRegProc();
	proc_manager.EndProc();
	return 0;*/
}

void *process_request(void *soap) 
{ 
	//pthread_detach(pthread_self()); 
	soap_serve((struct soap*)soap); 
	soap_destroy((struct soap*)soap); // dealloc C++ data 
	soap_end((struct soap*)soap); // dealloc data and clean up 
	soap_done((struct soap*)soap); // detach soap struct 
	free(soap); 
	return NULL; 
}

#if defined (__unix)	
int __ns1__Request(struct soap* sop, ns2__RequestMessageType *req, ns2__ResponseMessageType *rep)
#else
int __cdecl __ns1__Request(struct soap* sop, ns2__RequestMessageType *req, ns2__ResponseMessageType *rep)
#endif // unix
{
	if (!req->Request->ID || req->Request->ID->empty())
	{
		return SOAP_ERR;
	}
	int tmp = req->Header->Verb;
	if (_ns2__HeaderType_Verb__get != req->Header->Verb)
	{
		return SOAP_ERR;
	}

	ID_STR* id = req->Request->ID;		//获取的数据描述 实时表名+代码+字段名 设备的SCD代码 阵列的站所线系代码
// 	for (int i = 0; i < id.size(); i++)
// 	{
// 		printf("request ID %d = %s\n", i, id[i].c_str());
// 	}
	std::string request_type = req->Header->Noun;





	//填充rep
	///////////////////////////////////Responses header填充///////////////////////////////////////
	rep->Header = soap_new_ns2__HeaderType(sop,1);
// 	rep->Header = new ns2__HeaderType;
	rep->Header->Verb = _ns2__HeaderType_Verb__reply;
 	rep->Header->Noun = request_type;
// 	///////////////////////////////////Responses reply填充///////////////////////////////////////
	rep->Reply = soap_new_ns2__ReplyType(sop);
// 	rep->Reply = new ns2__ReplyType;
// 		//返回结果  OK  PARTIAL FAILED?待判断

// 
// 
// 	///////////////////////////////////Responses Payload填充///////////////////////////////////////
	rep->Payload = soap_new_ns2__PayloadType(sop);
	rep->Payload = new ns2__PayloadType;
	rep->Payload->Format = new std::string("getdata");

	//根据需求取数据
	if (request_type == "ArrayValues")
	{
		rep->Payload->__union_PayloadType = SOAP_UNION__ns2__union_PayloadType_ArrayValues;
		rep->Payload->union_PayloadType.ArrayValues = new std::vector<class ns3__ArrayValuesType * >;
		ns3__ArrayValuesType* ptr = new ns3__ArrayValuesType;

	}
	else if (request_type == "DeviceValues")
	{
		rep->Payload->__union_PayloadType = SOAP_UNION__ns2__union_PayloadType_DeviceValues;
		rep->Payload->union_PayloadType.DeviceValues = new std::vector<class ns3__DeviceValuesType * >;
		ns3__DeviceValuesType* ptr = new ns3__DeviceValuesType;

	}
	else if (request_type == "CommonValues" || request_type == "SimpleValues")
	{
		rep->Payload->__union_PayloadType = SOAP_UNION__ns2__union_PayloadType_CommonValues;
		rep->Payload->union_PayloadType.CommonValues = new std::vector<class ns3__CommonValuesType * >;
		ns3__CommonValuesType* ptr = new ns3__CommonValuesType;

	}

		int get_result = 0;
		RdbValueGet_B get_rdbdata(id->size());
	//组织多帧报文
// 	printf("request data_type %s request data: %s\n", request_type.c_str(), id->at(i).__item.c_str());

		get_result = get_rdbdata.Get_Data_From_Database(id,rep->Payload, request_type);

		if (get_result==0)
		{
			rep->Reply->Result = _ns2__ReplyType_Result__FAILED;
		}
		else if (get_result == 3*id->size())
		{
			rep->Reply->Result = _ns2__ReplyType_Result__OK;
		}
		else
		{
			rep->Reply->Result = _ns2__ReplyType_Result__PARTIAL;
		}

	
	return SOAP_OK;

}


#if defined (__unix)
int __ns1__Response(struct soap*, ns2__ResponseMessageType *ns2__ResponseMessage, struct __ns1__ResponseResponse &_param_1)
#else
int __cdecl __ns1__Response(struct soap*, ns2__ResponseMessageType *ns2__ResponseMessage, struct __ns1__ResponseResponse &_param_1)
#endif
{
	return SOAP_OK;

}
#if defined (__unix)
int __ns1__PublishEvent(struct soap *s,class ns2__EventMessageType *req,class ns2__ResponseMessageType *rep)
#else
int __cdecl __ns1__PublishEvent(struct soap *s,class ns2__EventMessageType *req,class ns2__ResponseMessageType *rep)
#endif
{
	return SOAP_OK;
}

#if !defined (ACE_WIN32)
int ProcessLock(const char *file, pid_t *pid, int fwr)
{
	int   fd = 0;
	pid_t pidw;

	struct flock lck;

	if( file == NULL || *file == 0 ) return(-1);

	fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);           //如果存在就返回
	if( fd < 0 ) return(-1);

	/* if( lockf(fd, F_TLOCK, 0) < 0 ) */

	lck.l_type = F_WRLCK;
	lck.l_start = 0;
	lck.l_whence = SEEK_SET;
	lck.l_len = 0;

	if( fcntl(fd, F_SETLK, &lck) < 0 )      //设置读写锁
	{
		if( pid ) read(fd, pid, sizeof(pid_t));     //写锁失败，读依然可以
		close(fd);
		return(-2);
	}

	/* 记录进程ID */
	if( fwr )
	{
		pidw = getpid();
		write(fd, &pidw, sizeof(pid_t));

		/* 刷新 */
		fsync(fd);
	}

	return(fd);
}
#endif


