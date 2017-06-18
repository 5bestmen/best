package com.corsair.api;

import net.sf.json.JSONArray;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2016/1/27.
 */
@RestController
public class SvgController {
    String realPath = "";

    public class Node {
        Node() {
            nodes = new ArrayList<>();
        }
        String text;
        String path;
        List<Node> nodes;

        public String getText() {
            return text;
        }

        public List<Node> getNodes() {
            return nodes;
        }

        public String getPath() {
            return path;
        }
    }

    @RequestMapping(value = "/svg", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> getSvg(HttpServletRequest request){

        realPath = request.getSession().getServletContext().getRealPath("/");

        Node root = new Node();
        getFiles(realPath+"svg", root);


        JSONArray array = JSONArray.fromObject(root.nodes);
        return new ResponseEntity<String>(array.toString(), HttpStatus.OK);
    }

    private void getFiles(String filePath, Node parent){
        File root = new File(filePath);
        File[] files = root.listFiles();
        for(File file:files){
            Node node = new Node();
            node.text = file.getName();
            node.path = file.getPath().substring(realPath.length());
            parent.nodes.add(node);
            if(file.isDirectory()) {
                getFiles(file.getAbsolutePath(), node);
            }

        }
    }
}
