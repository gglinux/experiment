#include <windows.h>
#include <iostream>

const unsigned short SIZE_OF_BUFFER = 2; //����������
unsigned short ProductID = 0;    //��Ʒ��
unsigned short ConsumeID = 0;    //�������ĵĲ�Ʒ��
unsigned short in = 0;      //��Ʒ��������ʱ�Ļ������±�
unsigned short out = 0;      //��Ʒ��������ʱ�Ļ������±�

int buffer[SIZE_OF_BUFFER];    //�������Ǹ�ѭ������
bool p_ccontinue = true;      //���Ƴ������
HANDLE Mutex;       //�����̼߳�Ļ���
HANDLE FullSemaphore;     //����������ʱ��ʹ�����ߵȴ�
HANDLE EmptySemaphore;     //����������ʱ��ʹ�����ߵȴ�

DWORD WINAPI Producer(LPVOID);    //�������߳�
DWORD WINAPI Consumer(LPVOID);    //�������߳�

int main()
{
    //�������������ź�
	//ע�⣬�����ź�����ͬ���ź����Ķ��巽����ͬ�������ź������õ���CreateMutex������ͬ���ź���
	//���õ���CreateSemaphore�����������ķ���ֵ���Ǿ����
    Mutex = CreateMutex(NULL,FALSE,NULL); 
	EmptySemaphore = CreateSemaphore(NULL,SIZE_OF_BUFFER,SIZE_OF_BUFFER,NULL);
    //���Ͼ��������޸ģ��������������
	//EmptySemaphore = CreateSemaphore(NULL,0,SIZE_OF_BUFFER-1,NULL);
    FullSemaphore = CreateSemaphore(NULL,0,SIZE_OF_BUFFER,NULL);

    //�����������ֵ�����Է��֣��������߸������������߸���ʱ��
    //�����ٶȿ죬�����߾����ȴ������ߣ���֮�������߾����ȴ� 
    const unsigned short PRODUCERS_COUNT = 5;  //�����ߵĸ���
    const unsigned short CONSUMERS_COUNT = 3;  //�����ߵĸ���

    //�ܵ��߳���
    const unsigned short THREADS_COUNT = PRODUCERS_COUNT+CONSUMERS_COUNT;

    HANDLE hThreads[THREADS_COUNT]; //���̵߳�handle
    DWORD producerID[PRODUCERS_COUNT]; //�������̵߳ı�ʶ��
    DWORD consumerID[CONSUMERS_COUNT]; //�������̵߳ı�ʶ��

	//�����������߳�
    for (int i=0;i<PRODUCERS_COUNT;++i){
        hThreads[i]=CreateThread(NULL,0,Producer,NULL,0,&producerID[i]);
        if (hThreads[i]==NULL) return -1;
    }
    //�����������߳�
    for (int i=0;i<CONSUMERS_COUNT;++i){
        hThreads[PRODUCERS_COUNT+i]=CreateThread(NULL,0,Consumer,NULL,0,&consumerID[i]);
        if (hThreads[i]==NULL) return -1;
    }

    while(p_ccontinue){
        if(getchar()){ //���س�����ֹ��������
            p_ccontinue = false;
        }
    }

    return 0;
}

//����һ����Ʒ����ģ����һ�£�������²�Ʒ��ID��
void Produce()
{
    std::cout << std::endl<< "Producing " << ++ProductID << " ... ";
    std::cout << "Succeed" << std::endl;
}

//���������Ĳ�Ʒ���뻺����
void Append()
{
    std::cerr << "Appending a product ... ";
    buffer[in] = ProductID;
    in = (in+1)%SIZE_OF_BUFFER;
    std::cerr << "Succeed" << std::endl;

    //�����������ǰ��״̬
    for (int i=0;i<SIZE_OF_BUFFER;++i){
        std::cout << i <<": " << buffer[i];
        if (i==in) std::cout << " <-- ����";
        if (i==out) std::cout << " <-- ����";
        std::cout << std::endl;
    }
}

//�ӻ�������ȡ��һ����Ʒ
void Take()
{
    std::cerr << "Taking a product ... ";
    ConsumeID = buffer[out];
	buffer[out] = 0;
    out = (out+1)%SIZE_OF_BUFFER;
    std::cerr << "Succeed" << std::endl;

    //�����������ǰ��״̬
    for (int i=0;i<SIZE_OF_BUFFER;++i){
        std::cout << i <<": " << buffer[i];
        if (i==in) std::cout << " <-- ����";
        if (i==out) std::cout << " <-- ����";
        std::cout << std::endl;
    }
}

//����һ����Ʒ
void Consume()
{
    std::cout << "Consuming " << ConsumeID << " ... ";
    std::cout << "Succeed" << std::endl;
}

//������
DWORD  WINAPI Producer(LPVOID lpPara)
{
    while(p_ccontinue){
        WaitForSingleObject(EmptySemaphore,INFINITE);	//p(empty);
        WaitForSingleObject(Mutex,INFINITE);	//p(mutex);
        Produce();
        Append();
        Sleep(1500);
        ReleaseMutex(Mutex);	//V(mutex);
        ReleaseSemaphore(FullSemaphore,1,NULL);	//V(full);
    }
    return 0;
}

//������
DWORD  WINAPI Consumer(LPVOID lpPara)
{
    while(p_ccontinue){
        WaitForSingleObject(FullSemaphore,INFINITE);	//P(full);
        WaitForSingleObject(Mutex,INFINITE);		//P(mutex);
        Take();
        Consume();
        Sleep(1500);
        ReleaseMutex(Mutex);		//V(mutex);
        ReleaseSemaphore(EmptySemaphore,1,NULL);		//V(empty);
    }
    return 0;
}
