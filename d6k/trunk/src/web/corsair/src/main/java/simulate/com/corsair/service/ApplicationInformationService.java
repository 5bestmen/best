package simulate.com.corsair.service;

import com.corsair.dao.ApplicationInformationDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by 洪祥 on 15/7/1.
 */
@Service("applicationInformationService")
public class ApplicationInformationService {
    @Autowired
    ApplicationInformationDao applicationInformationDao;

    public String getAppName() {
        return applicationInformationDao.getAppName();
    }

    public String getVersion() {
        return applicationInformationDao.getVersion();
    }
}
