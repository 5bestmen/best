package com.corsair.service;

import com.corsair.dao.ProcessDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by 洪祥 on 2015/11/06.
 */
@Service("ticketProcessService")
public class TicketProcessService {
    @Autowired
    private ProcessDao processDao;

    public List<Process> getProcessList(String name,String displayName){
        return processDao.getProcessList(name,displayName);
    }

    public Process getProcessById(String id){
        return processDao.findById(Process.class,id);
    }

    public void saveOrUpdateProcess(Process process){
        processDao.saveOrUpdate(process);
    }

    public void deleteProcess(String id){
        Process process = processDao.findById(Process.class,id);
        processDao.delete(process);
    }
}
