package simulate.com.corsair.servlet;

import net.sf.json.JSONArray;

import simulate.com.corsair.service.station.ArrayStatusService;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;


/**
 * Created by 洪祥 on 15/7/8.
 */
@WebServlet(name="JungleFlowServlet", urlPatterns="/JungleFlowServlet")
public class JungleFlowServlet extends BaseServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        ArrayStatusService arrayStatusService = (ArrayStatusService)super.getBean("arrayStatus");
        JSONArray array = JSONArray.fromObject(arrayStatusService.getJungleFlows());
        response.getOutputStream().write(array.toString().getBytes());
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
