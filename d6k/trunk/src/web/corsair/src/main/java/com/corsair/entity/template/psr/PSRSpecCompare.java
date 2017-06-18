package com.corsair.entity.template.psr;

import java.util.Comparator;
import java.util.Objects;

/**
 * Created by 洪祥 on 16/2/23.
 */
public class PSRSpecCompare implements Comparator {
    @Override
    public int compare(Object o1, Object o2) {
        PSRSpec spec1 = (PSRSpec)o1;
        PSRSpec spec2 = (PSRSpec)o2;

        if (Objects.equals(spec1.getIndex(), spec2.getIndex())) {
            return 0;
        }

        return spec1.getIndex().compareTo(spec2.getIndex());
    }
}
