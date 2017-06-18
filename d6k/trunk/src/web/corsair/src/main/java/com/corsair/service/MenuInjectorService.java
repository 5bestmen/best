package com.corsair.service;

import com.corsair.dao.StationDao;
import com.corsair.device.BasePSR;
import com.corsair.device.PhotovoltaicArray;
import com.rbac.entity.SysMenu;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/23.
 */
@Service("menuInjectorService")
public class MenuInjectorService {
    @Autowired
    private ScadaPSRService scadaPSRService;

    public List<SysMenu> inject(List<SysMenu> menuList) {
        Long PVArrayMenuID = -1L;
        Long beginID = 10000L;

        for (SysMenu menu : menuList) {
            if (menu.getName().contains("光伏") && menu.getIsDeleted() == 0) {
                PVArrayMenuID = menu.getId();
            }
        }

        if (PVArrayMenuID != -1L) {
            List<BasePSR> pvArrays = scadaPSRService.getPVArray();

            for (Integer i = 0, count = pvArrays.size(); i < count; i++) {
                SysMenu menu = new SysMenu();
                BasePSR array = pvArrays.get(i);
                menu.setId(beginID + i);
                menu.setName(array.getName());
                menu.setParentId(PVArrayMenuID);
                menu.setIsDeleted(0);
                menu.setIsShow(1);
                menu.setOrderSeq(i);
                menu.setChildrenCount(0);
                menu.setUrl("pvarray?PVArrayID=" + array.getId());
                menu.setParentName("光伏阵列");
                menu.setNeedCheck(1);
                menu.setChecked(false);
                menuList.add(menu);
            }
        }

        return menuList;
    }
}
