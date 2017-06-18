package com.corsair.util;

/**
 * Created by guo_zhiqi on 2016/1/5.
 */

import com.corsair.export.ExcelExportData;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.ss.util.CellRangeAddress;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Map.Entry;
import java.util.Set;

public class ExcelUtil {
    private static HSSFWorkbook wb;

    private static CellStyle titleStyle; // 标题行样式
    private static Font titleFont; // 标题行字体
    private static CellStyle dateStyle; // 日期行样式
    private static Font dateFont; // 日期行字体
    private static CellStyle headStyle; // 表头行样式
    private static Font headFont; // 表头行字体
    private static CellStyle contentStyle; // 内容行样式
    private static Font contentFont; // 内容行字体

    /**
     * 导出文件
     *
     * @param setInfo
     * @param outputExcelFileName
     * @return
     * @throws java.io.IOException
     */
    public static boolean export2File(ExcelExportData setInfo,
                                      String outputExcelFileName,String model) throws Exception {
        return FileUtil.write(outputExcelFileName, export2ByteArray(setInfo, model),
                true, true);
    }

    /**
     * 导出到byte数组
     *
     * @param setInfo
     * @return
     * @throws Exception
     */
    public static byte[] export2ByteArray(ExcelExportData setInfo, String model)
            throws Exception {
        return export2Stream(setInfo, model).toByteArray();
    }

    /**
     * 导出到流
     *
     * @param setInfo
     * @return
     * @throws Exception
     */
    public static ByteArrayOutputStream export2Stream(ExcelExportData setInfo, String model)
            throws Exception {
        init();

        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        Set<Entry<String, List<?>>> set = setInfo.getDataMap().entrySet();
        String[] sheetNames = new String[setInfo.getDataMap().size()];
        int sheetNameNum = 0;
        for (Entry<String, List<?>> entry : set) {
            sheetNames[sheetNameNum] = entry.getKey();
            sheetNameNum++;
        }
        HSSFSheet[] sheets = getSheets(setInfo.getDataMap().size(), sheetNames);
        int sheetNum = 0;
        for (Entry<String, List<?>> entry : set) {
            // Sheet
            List<?> objs = entry.getValue();

            // 标题行
            // createTableTitleRow(setInfo, sheets, sheetNum);

            // 日期行
            //  createTableDateRow(setInfo, sheets, sheetNum);

            // 表头
            creatTableHeadRow(setInfo, sheets, sheetNum);

            if ("firstModel".equals(model)) {
                createTableRemarkRow(setInfo, sheets, sheetNum);
            }
            // 表体
            String[] fieldNames = setInfo.getFieldNames().get(sheetNum);

            int rowNum = 2;
            for (Object obj : objs) {
                HSSFRow contentRow = sheets[sheetNum].createRow(rowNum);
                contentRow.setHeight((short) 300);
                if ("firstModel".equals(model)) {
                    mergeCells(setInfo
                            .getFieldNames().get(sheetNum), sheets[sheetNum]);
                } else {
                    mergeCellsModel2( sheets[sheetNum]);
                }

                HSSFCell[] cells = getCells(contentRow, setInfo.getFieldNames()
                        .get(sheetNum).length);
                int cellNum = 1; // 去掉一列序号，因此从1开始
                if (fieldNames != null) {
                    for (int num = 0; num < fieldNames.length; num++) {
                        Object value = ReflectionUtil.invokeGetterMethod(obj,
                                fieldNames[num]);
                        if (value == null||value.equals("null")) {
                            cells[cellNum].setCellValue("");
                        } else {
                            String ary = value.toString();
                                if (ary.indexOf(".") > 0&&ary.indexOf("/")<0) {
                                    Double d = Double.valueOf(ary);
                                    cells[cellNum].setCellValue(String.format("%.4f", d));
                                } else {
                                    cells[cellNum].setCellValue(ary);
                                }
                        }
                        cellNum++;
                    }
                }
                rowNum++;
            }
            adjustColumnSize(sheets, sheetNum, fieldNames); // 自动调整列宽
            sheetNum++;
        }
        wb.write(outputStream);
        return outputStream;
    }


    private static void mergeCells(String[] strings, HSSFSheet sheet) {
        int count = 0;
        for (int i = 1; i < 6; i++) {
            CellRangeAddress titleRanges = new CellRangeAddress(2 + count, 7 + count, 1, strings.length - 13);
            sheet.addMergedRegion(titleRanges);
            CellRangeAddress titleRange = new CellRangeAddress(2 + count, 4 + count, 2, strings.length - 12);
            sheet.addMergedRegion(titleRange);
            CellRangeAddress titleRange2 = new CellRangeAddress(5 + count, 7 + count, 2, strings.length - 12);
            sheet.addMergedRegion(titleRange2);

            sheet.addMergedRegion(new CellRangeAddress(32, 36, 1, 2));
            for (int j = 0; j <6 ; j++) {
                sheet.addMergedRegion(new CellRangeAddress(32+j, 32+j, 3, 5));

            }

            for (int j = 0; j < 6; j++) {
                //电流
                sheet.addMergedRegion(new CellRangeAddress(2 + count, 4 + count, 6 + j, strings.length + j - 8));
                sheet.addMergedRegion(new CellRangeAddress(5 + count, 7 + count, 6 + j, strings.length + j - 8));

                if(j==1){
                    sheet.addMergedRegion(new CellRangeAddress(2 + count, 4 + count, 10 ,10 ));
                    sheet.addMergedRegion(new CellRangeAddress(5 + count, 7 + count, 10 ,10));
                }else {
                    sheet.addMergedRegion(new CellRangeAddress(2 + count, 7 + count, 9 + j, strings.length + j - 5));
                }
            }
            count = count + 6;
        }
    }


    private static void mergeCellsModel2(HSSFSheet sheet) {
        int count = 0;
        CellRangeAddress titleRanges = new CellRangeAddress(2 + count, 7 + count, 1, 1);
        sheet.addMergedRegion(titleRanges);
        CellRangeAddress titleRange = new CellRangeAddress(2 + count, 4 + count, 2, 2);
        sheet.addMergedRegion(titleRange);
        CellRangeAddress titleRange2 = new CellRangeAddress(5 + count, 7 + count, 2, 2);
        sheet.addMergedRegion(titleRange2);

        sheet.addMergedRegion(new CellRangeAddress(2, 7, 16, 16));
        sheet.addMergedRegion(new CellRangeAddress(2, 7, 17, 17));
        for (int i = 1; i < 4; i++) {
            sheet.addMergedRegion(new CellRangeAddress(2, 7, 11 + i, 11 + i));
        }
        for (int j = 0; j < 19; j++) {
            sheet.addMergedRegion(new CellRangeAddress(0, 1, j, j));
        }
        count = count + 6;
    }

    /**
     * @Description: 初始化
     */
    private static void init() {
        wb = new HSSFWorkbook();

        titleFont = wb.createFont();
        titleStyle = wb.createCellStyle();
        dateStyle = wb.createCellStyle();
        dateFont = wb.createFont();
        headStyle = wb.createCellStyle();
        headFont = wb.createFont();
        contentStyle = wb.createCellStyle();
        contentFont = wb.createFont();

        initTitleCellStyle();
        initTitleFont();
        initDateCellStyle();
        initDateFont();
        initHeadCellStyle();
        initHeadFont();
        initContentCellStyle();
        initContentFont();
    }

    /**
     * @Description: 自动调整列宽
     */
    private static void adjustColumnSize(HSSFSheet[] sheets, int sheetNum,
                                         String[] fieldNames) {
        for (int i = 0; i < fieldNames.length + 1; i++) {
            sheets[sheetNum].autoSizeColumn(i, true);
        }
    }

    /**
     * @Description: 创建标题行(需合并单元格)
     */
    private static void createTableTitleRow(ExcelExportData setInfo,
                                            HSSFSheet[] sheets, int sheetNum) {
        CellRangeAddress titleRange = new CellRangeAddress(0, 0, 0, setInfo
                .getFieldNames().get(sheetNum).length);
        sheets[sheetNum].addMergedRegion(titleRange);
        HSSFRow titleRow = sheets[sheetNum].createRow(0);
        titleRow.setHeight((short) 800);
        HSSFCell titleCell = titleRow.createCell(0);
        titleCell.setCellStyle(titleStyle);
        titleCell.setCellValue(setInfo.getTitles()[sheetNum]);
    }

    /**
     * @Description: 创建日期行(需合并单元格)
     */
    private static void createTableDateRow(ExcelExportData setInfo,
                                           HSSFSheet[] sheets, int sheetNum) {
        CellRangeAddress dateRange = new CellRangeAddress(1, 1, 0, setInfo
                .getFieldNames().get(sheetNum).length);
        sheets[sheetNum].addMergedRegion(dateRange);
        HSSFRow dateRow = sheets[sheetNum].createRow(1);
        dateRow.setHeight((short) 350);
        HSSFCell dateCell = dateRow.createCell(0);
        dateCell.setCellStyle(dateStyle);
        // dateCell.setCellValue("导出时间：" + new
        // SimpleDateFormat("yyyy-MM-dd HH:mm:ss")
        // .format(new Date()));
        dateCell.setCellValue(new SimpleDateFormat("yyyy-MM-dd")
                .format(new Date()));
    }

    /**
     * @Description: 特殊注释(需合并单元格)
     */
    private static void createTableRemarkRow(ExcelExportData setInfo,
                                             HSSFSheet[] sheets, int sheetNum) {
        CellRangeAddress dateRange = new CellRangeAddress(1, 1, 6, setInfo
                .getFieldNames().get(sheetNum).length - 2);
        sheets[sheetNum].addMergedRegion(dateRange);
        HSSFRow dateRow = sheets[sheetNum].createRow(1);
        dateRow.setHeight((short) 350);
        HSSFCell[] cells = getCells(dateRow, setInfo.getFieldNames()
                .get(sheetNum).length);
        cells[6].setCellValue("以上数据按小时导出（时间标注峰谷平）");
    }

    /**
     * @Description: 创建表头行--行(需合并单元格)
     */
    private static void creatTableHeadRow(ExcelExportData setInfo,
                                          HSSFSheet[] sheets, int sheetNum) {
        // 表头
        HSSFRow headRow = sheets[sheetNum].createRow(0);
        headRow.setHeight((short) 350);
        // 序号列
        HSSFCell snCell = headRow.createCell(0);
        snCell.setCellStyle(headStyle);
        snCell.setCellValue("序号");
        // 列头名称
        for (int num = 1, len = setInfo.getColumnNames().get(sheetNum).length; num <= len; num++) {
            HSSFCell headCell = headRow.createCell(num);
            headCell.setCellStyle(headStyle);
            headCell.setCellValue(setInfo.getColumnNames().get(sheetNum)[num - 1]);
        }
    }


    /**
     * @Description: 创建表头列--列(需合并单元格)
     */
    private static void creatTableHeadColumn(ExcelExportData setInfo,
                                             HSSFSheet[] sheets, int sheetNum) {
        // 表头
        HSSFRow headRow = sheets[sheetNum].createRow(0);
        headRow.setHeight((short) 350);
        // 序号列
        HSSFCell snCell = headRow.createCell(0);
        snCell.setCellStyle(headStyle);
        snCell.setCellValue("序号");
        // 列头名称
        for (int num = 1, len = setInfo.getColumnNames().get(sheetNum).length; num <= len; num++) {
            //HSSFCell headCell = headRow.createCell(num);
            HSSFRow Row = sheets[sheetNum].createRow(num - 1);
            HSSFCell[] cells = getCells(Row, setInfo.getFieldNames()
                    .get(sheetNum).length);
            cells[1].setCellStyle(headStyle);
            cells[1].setCellValue(setInfo.getColumnNames().get(sheetNum)[num - 1]);
        }
    }

    /**
     * @Description: 创建所有的Sheet
     */
    private static HSSFSheet[] getSheets(int num, String[] names) {
        HSSFSheet[] sheets = new HSSFSheet[num];
        for (int i = 0; i < num; i++) {
            sheets[i] = wb.createSheet(names[i]);
        }
        return sheets;
    }

    /**
     * @Description: 创建内容行的每一列(附加一列序号)
     */
    private static HSSFCell[] getCells(HSSFRow contentRow, int num) {
        HSSFCell[] cells = new HSSFCell[num + 1];

        for (int i = 0, len = cells.length; i < len; i++) {
            cells[i] = contentRow.createCell(i);
            cells[i].setCellStyle(contentStyle);
        }

        // 设置序号列值，因为出去标题行和日期行，所有-2
        cells[0].setCellValue(contentRow.getRowNum() - 2);

        return cells;
    }

    /**
     * @Description: 初始化标题行样式
     */
    private static void initTitleCellStyle() {
        titleStyle.setAlignment(CellStyle.ALIGN_CENTER);
        titleStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
        titleStyle.setFont(titleFont);
        titleStyle.setFillBackgroundColor(IndexedColors.SKY_BLUE.index);
    }

    /**
     * @Description: 初始化日期行样式
     */
    private static void initDateCellStyle() {
        dateStyle.setAlignment(CellStyle.ALIGN_CENTER_SELECTION);
        dateStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
        dateStyle.setFont(dateFont);
        dateStyle.setFillBackgroundColor(IndexedColors.SKY_BLUE.index);
    }

    /**
     * @Description: 初始化表头行样式
     */
    private static void initHeadCellStyle() {
        headStyle.setAlignment(CellStyle.ALIGN_CENTER);
        headStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
        headStyle.setFont(headFont);
        headStyle.setFillBackgroundColor(IndexedColors.YELLOW.index);
        headStyle.setBorderTop(CellStyle.BORDER_MEDIUM);
        headStyle.setBorderBottom(CellStyle.BORDER_THIN);
        headStyle.setBorderLeft(CellStyle.BORDER_THIN);
        headStyle.setBorderRight(CellStyle.BORDER_THIN);
        headStyle.setTopBorderColor(IndexedColors.BLUE.index);
        headStyle.setBottomBorderColor(IndexedColors.BLUE.index);
        headStyle.setLeftBorderColor(IndexedColors.BLUE.index);
        headStyle.setRightBorderColor(IndexedColors.BLUE.index);
    }

    /**
     * @Description: 初始化内容行样式
     */
    private static void initContentCellStyle() {
        contentStyle.setAlignment(CellStyle.ALIGN_CENTER);
        contentStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
        contentStyle.setFont(contentFont);
        contentStyle.setBorderTop(CellStyle.BORDER_THIN);
        contentStyle.setBorderBottom(CellStyle.BORDER_THIN);
        contentStyle.setBorderLeft(CellStyle.BORDER_THIN);
        contentStyle.setBorderRight(CellStyle.BORDER_THIN);
        contentStyle.setTopBorderColor(IndexedColors.BLUE.index);
        contentStyle.setBottomBorderColor(IndexedColors.BLUE.index);
        contentStyle.setLeftBorderColor(IndexedColors.BLUE.index);
        contentStyle.setRightBorderColor(IndexedColors.BLUE.index);
        contentStyle.setWrapText(true); // 字段换行
    }

    /**
     * @Description: 初始化标题行字体
     */
    private static void initTitleFont() {
        titleFont.setFontName("华文楷体");
        titleFont.setFontHeightInPoints((short) 20);
        titleFont.setBoldweight(Font.BOLDWEIGHT_BOLD);
        titleFont.setCharSet(Font.DEFAULT_CHARSET);
        titleFont.setColor(IndexedColors.BLUE_GREY.index);
    }

    /**
     * @Description: 初始化日期行字体
     */
    private static void initDateFont() {
        dateFont.setFontName("隶书");
        dateFont.setFontHeightInPoints((short) 10);
        dateFont.setBoldweight(Font.BOLDWEIGHT_BOLD);
        dateFont.setCharSet(Font.DEFAULT_CHARSET);
        dateFont.setColor(IndexedColors.BLUE_GREY.index);
    }

    /**
     * @Description: 初始化表头行字体
     */
    private static void initHeadFont() {
        headFont.setFontName("宋体");
        headFont.setFontHeightInPoints((short) 10);
        headFont.setBoldweight(Font.BOLDWEIGHT_BOLD);
        headFont.setCharSet(Font.DEFAULT_CHARSET);
        headFont.setColor(IndexedColors.BLUE_GREY.index);
    }

    /**
     * @Description: 初始化内容行字体
     */
    private static void initContentFont() {
        contentFont.setFontName("微软雅黑");
        contentFont.setFontHeightInPoints((short) 10);
        contentFont.setBoldweight(Font.BOLDWEIGHT_NORMAL);
        contentFont.setCharSet(Font.DEFAULT_CHARSET);
        contentFont.setColor(IndexedColors.BLACK.index);
    }

    /**
     * Excel导出数据类
     *
     * @author jimmy
     *
     */

}