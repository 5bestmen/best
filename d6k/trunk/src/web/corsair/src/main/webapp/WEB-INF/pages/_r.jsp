<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>

<c:forEach var="currentItem" items="${sideBarList}" varStatus="vs">
    <%--没有子菜单的情况下只需要一个li--%>
    <c:choose>
        <c:when test="${fn:length(currentItem.children) == 0}">
            <li>
                <a href="${currentItem.url}"><i class="fa fa-tasks fa-fw"></i>${currentItem.text}</a>
            </li>
        </c:when>
        <c:otherwise>
            <%--有子菜单时，需要一个li套ul--%>
            <li>
                <a href="#"><i class="fa fa-windows fa-fw"></i>${currentItem.text}<span
                        class="fa arrow"></span></a>

                <c:choose>
                    <c:when test="${level == 1}">
                        <c:set var="levelString" value="nav nav-second-level"/>
                    </c:when>
                    <c:when test="${level == 2}">
                        <c:set var="levelString" value="nav nav-third-level"/>
                    </c:when>
                    <c:when test="${level == 3}">
                        <c:set var="levelString" value="nav nav-forth-level"/>
                    </c:when>
                    <c:otherwise>
                        <c:set var="levelString" value=""/>
                    </c:otherwise>
                </c:choose>

                <ul class="${levelString}">

                    <c:set var="level" value="${level + 1}" scope="request"/>
                    <c:set var="sideBarList" value="${currentItem.children}" scope="request"/>
                    <c:import url="_r.jsp"/>
                </ul>
            </li>
        </c:otherwise>
    </c:choose>
</c:forEach>
<c:set var="level" value="${level - 1}" scope="request"/>

