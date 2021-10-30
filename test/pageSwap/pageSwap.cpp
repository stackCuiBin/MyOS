/*
 * @Descripttion: 
 * @version: V0.01
 * @Author: Cuibb
 * @Date: 2021-10-29 16:58:01
 * @LastEditors: Cuibb
 * @LastEditTime: 2021-10-30 18:03:04
 */

#include <iostream>
#include <string>
#include <list>

using namespace std;

#define PAGE_DIR_NUM  (0xF + 1)
#define PAGE_SUB_NUM  (0xF + 1)
#define PAGE_NUM      (PAGE_DIR_NUM * PAGE_SUB_NUM)
#define FRAME_NUM     (4)
#define FP_NONE       (-1)

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
    int pid;    // the task which use the frame
    int pnum;   // the page which the frame hold
    int ticks;  // the ticks to mark the usage frequency

    FrameItem()
    {
        pid = FP_NONE;
        pnum = FP_NONE;
        ticks = 0xFF;
    }
};

class PageTable
{
    int* m_pt[PAGE_DIR_NUM];

public:
    PageTable()
    {
        for (int i = 0; i < PAGE_DIR_NUM; i++) {
            m_pt[i] = NULL;
        }
    }

    int& operator [] (int i)
    {
        if ( (i >= 0) && (i < length()) ) {
            int dir = ((i & 0xF0) >> 4);
            int sub = i & 0x0F;

            if ( m_pt[dir] == NULL ) {
                m_pt[dir] = new int[PAGE_SUB_NUM];

                if ( m_pt[dir] != NULL ) {
                    for (int j = 0; j < PAGE_SUB_NUM; j++) {
                        m_pt[dir][j] = FP_NONE;
                    }
                } else {
                    PrintFatialErr("Failed to allocate page table.", -1, -1);
                }
            }

            return m_pt[dir][sub];
        } else {
            exit(-1);
        }
    }

    int length()
    {
        return PAGE_NUM;
    }

    ~PageTable()
    {
        for (int i = 0; i < PAGE_DIR_NUM; i++) {
            delete[] m_pt[i];
        }
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
list<int> FIFOMoveOut;

int GetFrameItem()
{
    int ret = FP_NONE;

    for (int i = 0; i < FRAME_NUM; i++) {
        if ( FrameTable[i].pid == FP_NONE ) {
            ret = i;
            break;
        }
    }

    return ret;
}

void ClearFrameItem(int frame)
{
    bool flag = false;

    PrintLog("Frame page content write back to disk...");

    FrameTable[frame].pid = FP_NONE;
    FrameTable[frame].pnum = FP_NONE;
    FrameTable[frame].ticks = 0xFF;

    for (list<PCB*>::iterator iter = TaskTable.begin(); (iter != TaskTable.end()) && (flag == false); iter++) {
        PageTable& pt = (*iter)->getPageTable();
        for (int i = 0; i < pt.length(); i++) {
            if ( pt[i] == frame ) {
                pt[i] = FP_NONE;
                flag = true;
                break;
            }
        }
    }
}

int RandomFrame()
{
    int frame = rand() % FRAME_NUM;
    
    PrintLog("Selete random frame to swap out, frame " + std::to_string(frame));
    
    ClearFrameItem(frame);

    return frame;
}

int FIFOFrame()
{
    int frame = FIFOMoveOut.front();

    FIFOMoveOut.pop_front();

    PrintLog("Selete FIFO frame to swap out, frame " + std::to_string(frame));

    ClearFrameItem(frame);

    return frame;
}

int LRUFrame()
{
    int frame = 0;
    int ticks = FrameTable[frame].ticks;
    string s = "";

    for (int i = 0; i < FRAME_NUM; i++) {
        s += "frame " + std::to_string(i) + " : " + std::to_string(FrameTable[i].ticks) + "     ";
        if ( ticks > FrameTable[i].ticks ) {
            ticks = FrameTable[i].ticks;
            frame = i;
        }
    }

    PrintLog(s);
    PrintLog("Selete LRU frame to swap out, frame " + std::to_string(frame));

    ClearFrameItem(frame);

    return frame;
}

int SwapPage()
{
    // return RandomFrame();
    // return FIFOFrame();
    return LRUFrame();
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
    FrameTable[frame].pnum = page;
    FrameTable[frame].ticks = 0xFF;

    FIFOMoveOut.push_back(frame);

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

        FrameTable[pt[page]].ticks += 5;
    } else {
        PrintLog("Task-" + std::to_string(pid) + " is finished.");
    } 
}

void ReleaseFrameItem(int pid)
{
    for (int i = 0; i < FRAME_NUM; i++) {
        if ( FrameTable[i].pid == pid ) {
            FrameTable[i].pid = FP_NONE;
            FrameTable[i].pnum = FP_NONE;
            FrameTable[i].ticks = 0xFF;
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

    PrintLog("\n=========== RUNNING ===========\n");

    while(true) {
        for (int i = 0; i < FRAME_NUM; i++) {
            FrameTable[i].ticks--;
        }

        if ( !TaskTable.empty() ) {
            for(list<PCB*>::iterator iter = TaskTable.begin(); iter != TaskTable.end(); ) {
                PCB* pcb = *iter;

                if ( pcb->running() ) {
                    AccessPage(*pcb);
                    iter++;
                } else {
                    PrintLog("Task-" + std::to_string(pcb->getPID()) + " is finished, release page.");
                    ReleaseFrameItem(pcb->getPID());
                    iter = TaskTable.erase(iter);
                    delete pcb;
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