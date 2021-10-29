/*
 * @Descripttion: 
 * @version: V0.01
 * @Author: Cuibb
 * @Date: 2021-10-29 16:58:01
 * @LastEditors: Cuibb
 * @LastEditTime: 2021-10-30 00:32:37
 */

#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

#define PAGE_NUM   (0xFF + 1)
#define FRAME_NUM  (4)
#define FP_NONE    (-1)

void PrintLog(string s)
{
    cout << s << endl;
}

void PrintPageMap(int pid, int page, int frame)
{
    cout << "Task-" << pid << " : page " << page << " =====> frame " << frame << endl;
}

void PrintFatialErr(string s, int pid, int page)
{
    s += " Task-" + std::to_string(pid) + ": Page" + std::to_string(page);

    cout << s << endl;
    
    exit(-1);
}

struct FrameItem
{
    int pid;
    int page;

    FrameItem()
    {
        pid = FP_NONE;
        page = FP_NONE;
    }
};

class PageTable
{
    int m_pt[PAGE_NUM];

public:
    PageTable()
    {
        init();
    }

    void init()
    {
        for(int i = 0; i < length(); i++) {
            m_pt[i] = FP_NONE;
        }
    }

    int& operator [] (int i)
    {
        if ( (i >= 0) && (i < length()) ) {
            return m_pt[i];
        } else {
            exit(-1);
        }
    }

    int length()
    {
        return PAGE_NUM;
    }
};

class PCB
{
    int m_pid;
    PageTable m_pageTable;
    int* m_pageSerial;
    int m_pageCount;
    int m_next;

public:
    PCB(int pid)
    {
        m_pid = pid;
        m_next = 0;
        m_pageCount = rand() % 5 + 5;
        m_pageSerial = new int[m_pageCount];

        if ( m_pageSerial != NULL ) {
            for (int i = 0; i < m_pageCount; i++) {
                m_pageSerial[i] = rand() % 8;
            }
        } else {
            exit(-1);
        }
    }

    PageTable& getPageTable()
    {
        return m_pageTable;
    }

    void initPageTable()
    {
        m_pageTable.init();
    }

    int getPID()
    {
        return m_pid;
    }

    int getNextPage()
    {
        int ret = m_next;
        m_next++;

        if ( ret < m_pageCount ) {
            ret = m_pageSerial[ret];
        } else {
            ret = FP_NONE;
        }

        return ret;
    }

    bool running()
    {
        return (m_next < m_pageCount);
    }

    void printSerial()
    {
        string s = "Task-" + std::to_string(m_pid) + " page serial:";

        for (int i = 0; i < m_pageCount; i++) {
            s += " " + std::to_string(m_pageSerial[i]);
        }
        
        PrintLog(s);
    }

    ~PCB()
    {
        delete[] m_pageSerial;
    }
};

FrameItem FrameTable[FRAME_NUM];
list<PCB*> TaskTable;

int GetFrameItem()
{
    int ret = FP_NONE;

    for (int i = 0; i < FRAME_NUM; i++) {
        if ( FrameTable[i].page == FP_NONE ) {
            ret = i;
            break;
        }
    }

    return ret;
}

int RandPage()
{
    int ret = rand() % FRAME_NUM;
    bool flag = false;

    FrameTable[ret].pid = FP_NONE;
    FrameTable[ret].page = FP_NONE;

    PrintLog("Random selete a frame to swap out...");
    PrintLog("Frame page " + std::to_string(ret) + " content write back to disk...");

    for (list<PCB*>::iterator iter = TaskTable.begin(); (iter != TaskTable.end()) && (flag == false); iter++) {
        PageTable& pt = (*iter)->getPageTable();
        for (int i = 0; i < pt.length(); i++) {
            if ( pt[i] == ret ) {
                pt[i] = FP_NONE;
                flag = true;
                break;
            }
        }
    }

    return ret;
}

int SwapPage()
{
    return RandPage();
}

int RequestPage(int pid, int page)
{
    int frame = GetFrameItem();

    if ( frame != FP_NONE ) {
        PrintLog("Get a free frame to storage content : frame " + std::to_string(frame));
    } else {
        PrintLog("No free frame, need to swap page out ...");

        frame = SwapPage();
        if ( frame != FP_NONE ) {
            PrintLog("Succeed to allocate a frame");
        } else {
            PrintFatialErr("Failed to swap page out, ", pid, page);
        }
    }

    PrintLog("Load content from disk to Frame " + std::to_string(frame));

    FrameTable[frame].pid = pid;
    FrameTable[frame].page = page;

    return frame;
}

void AccessPage(PCB& pcb)
{
    int pid = pcb.getPID();
    int page = pcb.getNextPage();
    PageTable& pt = pcb.getPageTable();

    if ( page != FP_NONE ) {
        PrintLog("Task-" + std::to_string(pid) + " access to page " + std::to_string(page));

        if ( pt[page] != FP_NONE ) {
            PrintLog("Find target page in page table ...");
            PrintPageMap(pid, page, pt[page]);
        } else {
            PrintLog("Target page not found, need to request page ...");

            pt[page] = RequestPage(pid, page);
            if ( pt[page] != FP_NONE ) {
                PrintPageMap(pid, page, pt[page]);
            } else {
                PrintFatialErr("Failed to request page ...", pid, page);
            }
        }
    } else {
        PrintLog("Task-" + std::to_string(pid) + " is finished.");
    } 
}

void ReleasePage(int pid)
{
    for (int i = 0; i < FRAME_NUM; i++) {
        if ( FrameTable[i].pid == pid ) {
            FrameTable[i].pid = FP_NONE;
            FrameTable[i].page = FP_NONE;
        }
    }
}


int main()
{
    int index = 0;

    srand(time(0));

    TaskTable.push_back(new PCB(1));
    TaskTable.push_back(new PCB(2));

    PrintLog("Show task page serial:");
    for(list<PCB*>::iterator iter = TaskTable.begin(); iter != TaskTable.end(); iter++) {
        (*iter)->printSerial();
    }

    PrintLog("=========== RUNNING ===========");

    while(true) {
        if ( !TaskTable.empty() ) {
            for(list<PCB*>::iterator iter = TaskTable.begin(); iter != TaskTable.end(); ) {
                PCB* pcb = *iter;

                if ( pcb->running() ) {
                    AccessPage(*pcb);
                    iter++;
                } else {
                    PrintLog("Task-" + std::to_string(pcb->getPID()) + " is finished, release page.");
                    
                    pcb->initPageTable();
                    ReleasePage(pcb->getPID());

                    iter = TaskTable.erase(iter);
                }

                cin.get();
            }      
        } else {
            PrintLog("No running task, exit program.");
            break;
        }
    }

    return 0;
}