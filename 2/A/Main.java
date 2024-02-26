package Dsa;


import java.util.*;


public class Main {
    /**
     This function convert String date into int value based on counting days spent before our day comes.
     **/
    static int[] amountOfLeaps = new int[10000];
    public static int dateToInt(String s){
        int res = 0;
        String[] input = s.split("-");
        int year = Integer.parseInt(input[0]) - 1;
        res += year * 365 + amountOfLeaps[year];
        int month = Integer.parseInt(input[1]) - 1;
        for(int i = 1; i <= month; i++){
            switch(i){
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                    res += 31;
                    break;
                case 2:
                    if((year + 1) % 4 == 0 && (year + 1) % 100 != 0 || (year + 1) % 400 == 0)
                        res += 29;
                    else
                        res += 28;
                    break;
                case 4:
                case 6:
                case 9:
                case 11:
                    res += 30;
                    break;

            }
        }
        return (res + Integer.parseInt(input[2]));
    }

    /**
     Sorting algorithm based on putting transactions with the same date value in the same bucket then getting out data from buckets
     in chronological order
     **/
    public static ArrayList<Transaction> countingSort(ArrayList<Transaction> v){
        int maximumDateDay = v.get(0).getDate();
        for(int i = 1; i < v.size(); i++) // find amount of needed buckets arraylist
            maximumDateDay = Math.max(maximumDateDay, v.get(i).getDate());
        ArrayList<ArrayList<Transaction>> cnt = new ArrayList<>(); // buckets for counting sort
        for(int i = 0; i <= maximumDateDay; i++)
            cnt.add(new ArrayList<>());
        for(Transaction transaction: v)
            cnt.get(transaction.getDate()).add(transaction);
        ArrayList<Transaction> res = new ArrayList<>();
        for(int i = 0; i <= maximumDateDay; i++)
            res.addAll(cnt.get(i));
        return res;
    }

    /**
     * Find if transaction with certain day is already in the arraylist. If yes, return index of needed cell
     * if not, return -1 as a sign of non-existence
     * because initial arraylist is sorted by date it is possible to do search in Θ( logN )
     **/
    public static int indexByDate(ArrayList<Transaction> a, int curDate){
        int l = 0, r = a.size() - 1;
        while(l <= r){
            int m = (l + r) / 2;
            if(a.get(m).getDate() == curDate)
                return m;
            if(a.get(m).getDate() < curDate)
                l = m + 1;
            else
                r = m - 1;
        }
        return -1;
    }

    /**
     * implementation of quickSort algorithm
     **/

    public static int partition(ArrayList<Transaction> a, int l, int r){
        double pivot = a.get(r).getSpending();
        int i = l - 1;
        for(int j = l; j < r; j++){
            if(a.get(j).getSpending() < pivot){
                i++;

                Transaction temp = a.get(i);
                a.set(i, a.get(j));
                a.set(j, temp);
            }
        }
        Transaction temp = a.get(i + 1);
        a.set(i + 1, a.get(r));
        a.set(r, temp);
        return i + 1;
    }

    public static void quickSort(ArrayList<Transaction> a, int l, int r){
        if(l >= r)
            return;
        int p = partition(a, l, r);
        quickSort(a, l, p - 1);
        quickSort(a, p + 1, r);
    }

    /**
     * This function calculate median in current window
     **/

    public static double doubleMedian(Queue<Transaction> q, int trailingSIze){
        ArrayList<Transaction> v = new ArrayList<>();
        while(!q.isEmpty()){ // put data from queue to arraylist for next sorting and fast access to middle element
            Transaction t = q.poll();
            int index = indexByDate(v, t.getDate());
            if(index > -1)
                v.set(index, new Transaction(t.getDate(), v.get(index).getSpending() + t.getSpending()));
            else
                v.add(t);
        }
        quickSort(v, 0, v.size() - 1); // sort by spending
        int am0 = trailingSIze - v.size(); // amount of days without spending which are not included in v-arraylist
        if(trailingSIze % 2 == 1)
            return (((trailingSIze / 2) - am0 >= 0) ? 2 * v.get((trailingSIze / 2) - am0).getSpending() : 0);
        double x = (((trailingSIze / 2) - am0) >= 1 ? v.get((trailingSIze / 2) - am0 - 1).getSpending() : 0);
        double y = (((trailingSIze / 2) - am0) >= 0 ? v.get((trailingSIze / 2) - am0).getSpending() : 0);
        return (x + y);
    }

    public static void main(String[] args) {
        amountOfLeaps[0] = 0;
        for(int i = 1; i < 10000; i++){
            amountOfLeaps[i] = amountOfLeaps[i - 1];
            if(i % 4 == 0 && i % 100 != 0 || i % 400 == 0)
                amountOfLeaps[i]++;
        }
        Scanner sc = new Scanner(System.in);
        int amountOfAlerts = 0; // variable which represents total amount of frauds detected
        int n = sc.nextInt(), d = sc.nextInt(); // amount of Transactions in total and amount of trailing days;
        ArrayList<Transaction> data = new ArrayList<>();
        for(int i = 0; i <= n; i++){ // parse input data
            String str = sc.nextLine();
            String[] s = str.split(" ");
            if(i > 0){
                s[1] = s[1].replace("$", ""); // there is no need in currency sign - only value
                data.add(new Transaction(dateToInt(s[0]), Double.parseDouble(s[1])));
            }
        }
        int minimumDateDay = data.get(0).getDate();
        for(int i = 1; i < data.size(); i++) //find transaction with the smallest date
            minimumDateDay = Math.min(minimumDateDay, data.get(i).getDate());
        for(Transaction transaction : data) // Data compression by setting starting point to our least day
            transaction.setDate(transaction.getDate() - minimumDateDay);
        data = countingSort(data); // sort data by date
        Queue<Transaction> trailing = new LinkedList<>();
        int curIndex = 0; // index of current transaction in data arraylist
        for(; curIndex < data.size() && data.get(curIndex).getDate() < d; curIndex++) // initial state of trailing days queue
            trailing.add(data.get(curIndex));
        while(curIndex < data.size()){ // move window and calculate amount of alerts per current day
            while(!trailing.isEmpty() && data.get(curIndex).getDate() - trailing.peek().getDate() > d)
                trailing.poll();
            int curDay = data.get(curIndex).getDate();
            Queue<Transaction> qTemp = new LinkedList<>(trailing); // clone trailing queue to put it in the method
            double mid = doubleMedian(qTemp, d), inTotal = 0;
            while(curIndex < data.size() && data.get(curIndex).getDate() == curDay){
                inTotal += data.get(curIndex).getSpending();
                if(inTotal >= mid) amountOfAlerts++;
                curIndex++;
            }
            trailing.add(new Transaction(curDay, inTotal));
        }
        System.out.println(amountOfAlerts);
    }
}

/**
 This class consists of date when current transaction was made, which is integer value of days spent before our date comes, and decimal value of current
 spending
 **/
class Transaction{
    private int date;
    private double spending;

    Transaction(int date, double spending){
        this.date = date;
        this.spending = spending;
    }
    public int getDate(){
        return this.date;
    }
    public double getSpending(){
        return this.spending;
    }
    public void setDate(int date){
        this.date = date;
    }
}