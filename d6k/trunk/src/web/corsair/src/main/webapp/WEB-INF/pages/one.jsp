<%--
  Created by IntelliJ IDEA.
  User: ChengXi
  Date: 2015/8/7
  Time: 14:26
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %>
<html>
<head>
  <meta charset="utf-8">
  <title>${sessionScope.APP_NAME}-一次接线SVG</title>
  <link href="resources/css/bootstrap.min.css" rel="stylesheet">
  <link href="resources/css/metisMenu.min.css" rel="stylesheet">
  <link href="resources/css/sb-admin-2.css" rel="stylesheet">
  <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
  <link href="resources/css/style.css" rel="stylesheet" type="text/css">

  <script src="resources/js/jquery.min.js"></script>
  <script src="resources/dist/js/bootstrap-treeview.min.js"></script>
  <script src="resources/js/bootstrap.min.js"></script>
  <script src="resources/js/metisMenu.js"></script>
  <script src="resources/js/sb-admin-2.js"></script>

</head>
<body>

<div id="wrapper">
  <jsp:include page="meun.jsp"/>

  <div id="page-wrapper">

    <div class="row">

      <div class="row">

        <div style="width: 100%;background: #fff;text-align: center;">

          <button class="svgZoomBtn btn" zoom="1" style="width: 15%" data-inline="true">放大</button>
          <button class="svgZoomBtn btn" zoom="-1" style="width: 15%" data-inline="true">缩小</button>
          <button class="svgMoveBtn btn" move="left" style="width: 15%" data-inline="true">左移</button>
          <button class="svgMoveBtn btn" move="right" style="width: 15%" data-inline="true">右移</button>
          <button class="svgMoveBtn btn" move="up" style="width: 15%" data-inline="true">上移</button>
          <button class="svgMoveBtn btn" move="down" style="width: 15%" data-inline="true">下移</button>
        </div>
      </div>
      <div class="row">
        <div class="col-md-12">
          <div id="svgdiv" class="panel panel-primary" id="map-0">
            <embed id="svgEmbed" src="svg/光伏集控中心/光伏电站集控中心.svg" type="image/svg+xml"
                   onload="initializeSvgDocument()"></embed>
          </div>
        </div>
      </div>
    </div>


  </div>

  <script>
    // 所有量测量
    var measures = {};

    // 所有图形对象  Meas_Ref的ObjectID和图元ID对应关系
    var objects = {};

    var int = self.setInterval(function () {
      ajaxStart()
    }, 60000);

    function initializeSvgDocument() {
      svgDocument = document.getElementById("svgEmbed").getSVGDocument();

      $.post('/api/svg', {}, function (response) {
        getTree(response);
      }, 'json');


      getAllObjects();
      ajaxStart();

    }

    $(".svgMoveBtn").click(function () {

      var viewBoxValues = svgDocument.getElementsByTagName('svg')[0].getAttribute('viewBox').split(' ');

      viewBoxValues[0] = parseFloat(viewBoxValues[0]);
      viewBoxValues[1] = parseFloat(viewBoxValues[1]);

      var move = $(this).attr("move");
      switch (move) {
        case 'left':
          viewBoxValues[0] += 20;
          break;
        case 'right':
          viewBoxValues[0] -= 20;
          break;
        case 'up':
          viewBoxValues[1] += 20;
          break;
        case 'down':
          viewBoxValues[1] -= 20;
          break;
      }

      svgDocument.getElementsByTagName('svg')[0].setAttribute('viewBox', viewBoxValues.join(' '));
    })

    $(".svgZoomBtn").click(function () {

      var zoom = $(this).attr("zoom");
      var cWidth = $("#svgEmbed").width();
      var cHeight = $("#svgEmbed").height();
      if (zoom > 0) {
        cWidth += cWidth;
        cHeight += cHeight;
      } else if (zoom < 0) {
        cWidth -= cWidth * 0.5;
        cHeight -= cHeight * 0.5;
      } else {
        //原大小
      }

      $("#svgEmbed").width(cWidth);
      $("#svgEmbed").height(cHeight);
    })

    function getData(id) {
      var dataElement = svgDocument.getElementById(id);

      if (dataElement) {
        return get_firstchild(dataElement).innerHTML;
      }

    }

    function get_firstchild(node) {
      var x = node.firstChild;
      while (x && x.nodeType != 1) {
        x = x.nextSibling;
      }
      return x;
    }

    function getLayerElements(Layer) {
      var layerelements = new Array();
      var Element = svgDocument.getElementById(Layer);
      var x = Element.firstElementChild;
      layerelements.push(x);
      for (var i = 1; i < Element.childElementCount; i++) {
        layerelements.push(x.nextElementSibling);
        x = x.nextElementSibling;
      }
      return layerelements;
    }

    // 递归函数，遍历parent参数的所有子节点, procfunc参数为遍历中的处理过程
    function getChildNodes(parent, procfunc) {

      var child = get_firstchild(parent);
      if (child != null) {
        procfunc(child, parent);
        getChildNodes(child, procfunc);
        while (child.nextElementSibling != null) {
          child = child.nextElementSibling;
          procfunc(child, parent);
          getChildNodes(child, procfunc);

        }
      }
    }

    // 遍历过程发现图形对象保存到map中
    function procGetObject(child, parent) {

      var findPSR = false;
      var findMeas = false;
      var measObjectIDs = new Array();
      if (child.tagName == "metadata") {

        var x = child.childNodes;
        for (var i = 0; i < x.length; i++) {
          if (x[i].nodeName == "cge:PSR_Ref") {
            findPSR = true;
          } else if (x[i].nodeName == "cge:Meas_Ref") {
            findMeas = true;
            measObjectIDs.push(x[i].getAttribute("ObjectID"));
          }
        }

        // PSR_Ref和Meas_Ref同时存在即认为图形对象
        if (findPSR && findMeas) {
          for (var i = 0; i < measObjectIDs.length; i++) {
            objects[measObjectIDs[i]] = parent.getAttribute("id");
          }
        } else if (!findPSR && findMeas) { // 只有Meas_Ref存在认为量测对象
          for (var i = 0; i < measObjectIDs.length; i++) {
            measures[measObjectIDs[i]] = parent.getAttribute("id");
          }
        }
      }
    }

    function modifyStatus(id, data) {
      var dataElement = svgDocument.getElementById(id)
      if (dataElement) {
        var useNode = get_firstchild(dataElement);
        var status = useNode.getAttribute('xlink:href');
        var pre = status.split('@');
        useNode.setAttribute('xlink:href', pre[0] + '@' + data);
      }
    }

    // 获取svg中所有图元对象
    function getAllObjects() {

      objects = {};
      measures = {};

      getChildNodes(get_firstchild(svgDocument), procGetObject);
    }

    function getAllBreakers() {
      return getLayerElements("Breaker_Layer");
    }

    function modifyData(id, data) {
      var dataElement = svgDocument.getElementById(id);
      if (dataElement) {

        var textNode = get_firstchild(dataElement);

        textNode.firstChild.textContent = data;
      }
    }

    function updateMeasurement() {

      var measureids = {};
      var measurementData = new Array();
      for (var m in measures) {
        var measurement = {};
        var split = m.split(" ");

        measureids[split[1]] = m;

        measurement.measurementID = split[1].replace(/(^\s*)|(\s*$)/g, '');

        measurementData.push(measurement);
      }

      $.post('/measurementServlet', {measurementData: JSON.stringify(measurementData)}, function (response) {
        for (var i in response) {
          modifyData(measures[measureids[response[i].key]], response[i].data, 'y');
        }
      }, 'json');
    }

    function updateObjects() {

      var objectids = {};
      var objectData = new Array();
      for (var o in objects) {
        var measurement = {};
        var split = o.split(" ");

        objectids[split[1]] = o;

        measurement.measurementID = split[1].replace(/(^\s*)|(\s*$)/g, '');

        objectData.push(measurement);
      }

      $.post('/measurementServlet', {measurementData: JSON.stringify(objectData)}, function (response) {
        for (var i in response) {
          modifyStatus(objects[objectids[response[i].key]], response[i].data);
        }
      }, 'json');
    }

    function log(msg) {
      console.log(msg);
    }

    function ajaxStart() {
      updateMeasurement();
      updateObjects();
    }

    function getTree(data) {

      $('#tree').treeview({
        data: data,
        onNodeSelected: function (event, data) {

          if (data.text.endWith("svg")) {
            $('#svgEmbed').remove();
            $('<embed id="svgEmbed" src="' + data.path + '" type="image/svg+xml" onload="initializeSvgDocument()"></embed>').appendTo('#svgdiv');
          }
        },
      });
    }

    String.prototype.endWith = function (endStr) {
      var d = this.length - endStr.length;
      return (d >= 0 && this.lastIndexOf(endStr) == d)
    }

    /** Event handler for mouse wheel event.
     *鼠标滚动事件
     */
    var scrollFunc = function (event) {
      var delta = 0;
      if (!event) /* For IE. */
        event = window.event;
      if (event.wheelDelta) { /* IE/Opera. */
        delta = event.wheelDelta / 120;
      } else if (event.detail) {
        /** Mozilla case. */
        /** In Mozilla, sign of delta is different than in IE.
         * Also, delta is multiple of 3.
         */
        delta = -event.detail / 3;
      }
      /** If delta is nonzero, handle it.
       * Basically, delta is now positive if wheel was scrolled up,
       * and negative, if wheel was scrolled down.
       */
      if (delta)
        handle(delta);
      /** Prevent default actions caused by mouse wheel.
       * That might be ugly, but we handle scrolls somehow
       * anyway, so don't bother here..
       */
      if (event.preventDefault)
        event.preventDefault();
      event.returnValue = false;
    }

    /*注册滚轮事件*/
    if (document.getElementById("svgEmbed").addEventListener) {
      document.getElementById("svgEmbed").addEventListener('DOMMouseScroll', scrollFunc, false);
    }//W3C
    document.getElementById("svgEmbed").onmousewheel = scrollFunc;//IE/Opera/Chrome/Safari


    /** This is high-level function.
     * It must react to delta being more/less than zero.
     */
    var handle = function (delta) {

      var zoom = $(this).attr("zoom");
      var cWidth = $("#svgEmbed").width();
      var cHeight = $("#svgEmbed").height();
      if (delta > 0) {
        cWidth += cWidth;
        cHeight += cHeight;
      } else {
        cWidth -= cWidth * 0.5;
        cHeight -= cHeight * 0.5;
      }

      $("#svgEmbed").width(cWidth);
      $("#svgEmbed").height(cHeight);
    }

  </script>

</body>
</html>


